# Autonomous-Space-Habitat-Operations-System-ASHOS-

Full-stack reference implementation for an AI-assisted, safety-first space habitat control stack.

## Layout
- **c_core/**: Real-time C controllers for life support, power, thermal, and watchdog safety.
- **cpp_engine/**: High-performance simulation engine with physics, thermal models, comm simulation, and HAL bridge.
- **python_ai/**: AI/analytics layer exposing anomaly detection and policy recommendations.
- **java_backend/**: Spring Boot mission control with telemetry endpoints and safety validation.
- **config/**: Habitat configuration, safety limits, and canned scenarios.
- **scripts/**: Orchestration for start-up, scenarios, health checks, and backup/restore.
- **tests/**: Stubs for unit/integration test suites per language.

Use `scripts/start_all.sh` to build and launch the simulation, AI server, and backend (Maven required for Java).
