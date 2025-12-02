"""Lightweight AI service that exposes recommendations over HTTP."""
import json
from http.server import BaseHTTPRequestHandler, HTTPServer
from typing import Dict

from anomaly_detection import anomaly_score
from policy_rl import recommend_power_split


class AIRequestHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        content_length = int(self.headers.get("Content-Length", 0))
        body = self.rfile.read(content_length)
        payload = json.loads(body.decode())
        telemetry = payload.get("telemetry", {})

        score = anomaly_score(telemetry)
        recommendation = recommend_power_split(telemetry)
        response: Dict[str, object] = {
            "anomaly_score": score,
            "recommendation": recommendation,
        }
        self.send_response(200)
        self.send_header("Content-Type", "application/json")
        self.end_headers()
        self.wfile.write(json.dumps(response).encode())


def serve(host: str = "0.0.0.0", port: int = 5001):
    server = HTTPServer((host, port), AIRequestHandler)
    print(f"AI server listening on {host}:{port}")
    server.serve_forever()


if __name__ == "__main__":
    serve()
