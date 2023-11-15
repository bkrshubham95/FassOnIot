package com.example.demo.Model;

import com.example.demo.Model.CommandDB;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonPropertyOrder;
import lombok.*;

import java.util.List;

@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
@Builder
@JsonInclude(JsonInclude.Include.NON_NULL)
@JsonPropertyOrder({"mode", "execCommands", "waitForExit"})
public class Command {
    @JsonProperty("mode")
    String mode;
    @JsonProperty("execCommands")
    String[] execCommands;
    @JsonProperty("waitForExit")
    String waitForExit;

    @JsonIgnore
    public CommandDB toCommandDB() {
        return CommandDB.builder()
                .execCommands(List.of(execCommands))
                .mode(mode)
                .waitForExit(waitForExit)
                .build();
    }
}
