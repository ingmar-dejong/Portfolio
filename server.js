'use strict';

const http = require('http');

const PORT = Number(process.env.PORT || 8080);

function sendJson(response, statusCode, payload) {
  response.writeHead(statusCode, {
    'Content-Type': 'application/json; charset=utf-8',
    'Access-Control-Allow-Origin': '*',
    'Access-Control-Allow-Methods': 'POST, OPTIONS',
    'Access-Control-Allow-Headers': 'Content-Type'
  });

  response.end(JSON.stringify(payload));
}

function normalizeString(value, fallback = '') {
  return typeof value === 'string' ? value : fallback;
}

function normalizeNumber(value, fallback = 0) {
  return Number.isFinite(value) ? value : fallback;
}

function normalizeBoolean(value, fallback = false) {
  return typeof value === 'boolean' ? value : fallback;
}

function buildNpcDecision(payload) {
  const npcId = normalizeString(payload.npc_id, 'UnknownNPC');
  const playerReputation = normalizeNumber(payload.player_reputation, 0);
  const location = normalizeString(payload.location, 'UnknownLocation');
  const activeAlert = normalizeBoolean(payload.active_alert, false);
  const context = payload.context && typeof payload.context === 'object' ? payload.context : {};

  let mood = 'calm';
  let threatScore = 0.15;
  let recommendedAction = 'Ignore';
  let replyText = `${npcId} remains calm in ${location}.`;

  if (activeAlert || playerReputation <= -25) {
    mood = 'aggressive';
    threatScore = 0.9;
    recommendedAction = 'RaiseAlarm';
    replyText = `${npcId} identifies the player as a direct threat.`;
  } else if (playerReputation < 10) {
    mood = 'suspicious';
    threatScore = 0.65;
    recommendedAction = 'WarnPlayer';
    replyText = `${npcId} issues a warning and keeps distance.`;
  }

  if (context.quest_state === 'Trespassing') {
    mood = 'aggressive';
    threatScore = Math.max(threatScore, 0.95);
    recommendedAction = 'RaiseAlarm';
    replyText = `${npcId} reacts to the player trespassing in ${location}.`;
  }

  return {
    mood,
    threat_score: threatScore,
    recommended_action: recommendedAction,
    reply_text: replyText
  };
}

function handleNpcDecision(request, response) {
  let rawBody = '';

  request.on('data', (chunk) => {
    rawBody += chunk;

    if (rawBody.length > 1024 * 1024) {
      request.destroy();
    }
  });

  request.on('end', () => {
    let payload;

    try {
      payload = rawBody ? JSON.parse(rawBody) : {};
    } catch {
      sendJson(response, 400, { error: 'Invalid JSON payload.' });
      return;
    }

    const decision = buildNpcDecision(payload);
    console.log('[external-ai] request:', payload);
    console.log('[external-ai] response:', decision);
    sendJson(response, 200, decision);
  });
}

const server = http.createServer((request, response) => {
  if (request.method === 'OPTIONS') {
    sendJson(response, 204, {});
    return;
  }

  if (request.method === 'GET' && request.url === '/health') {
    sendJson(response, 200, { ok: true, service: 'external-ai-sample-backend' });
    return;
  }

  if (request.method === 'POST' && request.url === '/api/npc/decision') {
    handleNpcDecision(request, response);
    return;
  }

  sendJson(response, 404, { error: 'Route not found.' });
});

server.listen(PORT, () => {
  console.log(`[external-ai] backend listening on http://127.0.0.1:${PORT}`);
});
