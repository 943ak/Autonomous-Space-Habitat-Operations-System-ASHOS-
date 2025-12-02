package com.ashos.missioncontrol;

import java.time.Instant;
import java.util.ArrayDeque;
import java.util.Deque;
import java.util.Optional;

public class CommandQueue {
    public static class CommandRecord {
        public final Instant timestamp = Instant.now();
        public final String source;
        public final String payload;

        public CommandRecord(String source, String payload) {
            this.source = source;
            this.payload = payload;
        }
    }

    private final Deque<CommandRecord> queue = new ArrayDeque<>();

    public synchronized void enqueue(String source, String payload) {
        queue.addLast(new CommandRecord(source, payload));
    }

    public synchronized Optional<CommandRecord> dequeue() {
        return queue.isEmpty() ? Optional.empty() : Optional.of(queue.removeFirst());
    }

    public synchronized int size() {
        return queue.size();
    }
}
