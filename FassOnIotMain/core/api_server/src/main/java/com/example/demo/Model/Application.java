package com.example.demo.Model;

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
@JsonPropertyOrder({"name", "version", "runtime", "resources", "dependencies", "platform", "containerized", "commands"})
public class Application {
    @JsonProperty("name")
    String name;

    @JsonProperty("version")
    String version;

    @JsonProperty("runtime")
    String runtime;
//     String[] rpt;
//     String rst;
    @JsonProperty("resources")
Resources resources;
//
//    @SuppressWarnings("unchecked")
//    @JsonProperty("resources")
//    private void unpackNested(Map<String,Object> resources) {
//        this.rpt = (String)resources.get("publishTo");
//        this.rst = (String)resources.get("subscribeTo");
////        Map<String,String> owner = (Map<String,String>)brand.get("owner");
////        this.ownerName = owner.get("name");
//    }
    @JsonProperty("dependencies")
    String dependencies;
    @JsonProperty("platform")
    String platform;
    @JsonProperty("containerized")
    String containerized;
    @JsonProperty("commands")
    Command[] commands;

    @JsonIgnore
    public ApplicationDB toApplicationDB(String extractedPath) {
        CommandDB[] commandDBs = new CommandDB[commands.length];

        for(int i = 0; i <commands.length; i++) {
            commandDBs[i] = commands[i].toCommandDB();
        }
        return ApplicationDB.builder()
                .name(name)
                .version(version)
                .runtimeEnvironment(runtime)
                .isContainerized("YES".equalsIgnoreCase(containerized))
                .dependencies(dependencies)
                .requiredPlatform(platform)
                .subsTopic(List.of(resources.subscribeTo))
                .pubsTopic(List.of(resources.publishTo))
                .status("INITIALIZED")
                .deploymentEngineReportedStatus("NOT AVAILABLE")
                .isProcessed(false)
                .extractedPath(extractedPath).build();
    }
}

