package com.ashos.missioncontrol;

import java.util.Map;
import java.util.concurrent.ConcurrentLinkedQueue;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/telemetry")
public class TelemetryController {
    private final ConcurrentLinkedQueue<Map<String, Object>> buffer = new ConcurrentLinkedQueue<>();

    @PostMapping
    public ResponseEntity<Void> ingest(@RequestBody Map<String, Object> telemetry) {
        buffer.add(telemetry);
        return ResponseEntity.accepted().build();
    }

    @GetMapping
    public ResponseEntity<Map<String, Object>> latest() {
        Map<String, Object> latest = buffer.peek();
        if (latest == null) {
            return ResponseEntity.noContent().build();
        }
        return ResponseEntity.ok(latest);
    }
}
