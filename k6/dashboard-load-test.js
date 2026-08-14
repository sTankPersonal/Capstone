import http from 'k6/http';
import { check, sleep } from 'k6';

const BASE_URL = __ENV.BASE_URL || 'http://localhost:18080';
const MODE = __ENV.MODE || 'load';
const POOL_SIZE = parseInt(__ENV.POOL_SIZE || '500', 10);
const RUN_ID = Date.now();

function parseVusList() {
  return (__ENV.VUS_LIST || '50,100,200,500')
    .split(',')
    .map((v) => Number(v.trim()))
    .filter((v) => Number.isFinite(v) && v > 0);
}

export const options = MODE === 'endurance'
  ? {
      scenarios: {
        endurance: {
          executor: 'constant-vus',
          vus: parseInt(__ENV.ENDURANCE_VUS || '50', 10),
          duration: __ENV.DURATION || '2h',
        },
      },
    }
  : {
      scenarios: {
        load: {
          executor: 'ramping-vus',
          startVUs: 0,
          stages: parseVusList().flatMap((vus) => [
            { target: vus, duration: '30s' }, // ramp
            { target: vus, duration: '1m' },  // hold and measure
          ]),
          gracefulRampDown: '10s',
        },
      },
    };

// Registers a pool of throwaway users up front (registering already logs
// them in via a Set-Cookie), so VUs can hammer the dashboard without each
// one paying registration cost mid-test.
export function setup() {
  const cookies = [];
  for (let i = 0; i < POOL_SIZE; i++) {
    const res = http.post(`${BASE_URL}/auth/register`, {
      first_name: 'Load',
      last_name: `Test${i}`,
      email: `loadtest_user_${i}_${RUN_ID}@example.com`,
      password: 'LoadTest123!',
    }, { redirects: 0 });
    const cookie = res.cookies.userId && res.cookies.userId[0];
    if (cookie) {
      cookies.push(cookie.value);
    }
  }
  if (cookies.length === 0) {
    throw new Error('Setup failed: could not register/authenticate any test users.');
  }
  return { cookies };
}

// No thresholds/asserts on purpose — this is for observing throughput and
// latency under load, not gating the build.
export default function (data) {
  const cookie = data.cookies[__VU % data.cookies.length];
  const res = http.get(`${BASE_URL}/user/dashboard`, {
    headers: { Cookie: `userId=${cookie}` },
  });
  check(res, { 'dashboard responded 200': (r) => r.status === 200 });
  sleep(1);
}
