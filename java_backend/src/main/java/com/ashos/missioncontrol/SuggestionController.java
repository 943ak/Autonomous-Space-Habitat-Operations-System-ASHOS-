package com.ashos.missioncontrol;

import java.util.Map;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/suggestions")
public class SuggestionController {
    private final SafetyRules safetyRules = new SafetyRules();
    private final CommandQueue commandQueue = new CommandQueue();

    @PostMapping
    public ResponseEntity<String> accept(@RequestBody Map<String, Object> suggestion) {
        if (!safetyRules.validate(suggestion)) {
            return ResponseEntity.badRequest().body("Rejected by safety rules");
        }
        commandQueue.enqueue("ai", suggestion.toString());
        return ResponseEntity.accepted().body("Queued");
    }
}
