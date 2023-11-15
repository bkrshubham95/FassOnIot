package com.example.demo.Model;

import lombok.*;

import jakarta.persistence.*;

import java.util.List;

@Getter
@Setter
@Entity
@Builder
@NoArgsConstructor
@AllArgsConstructor
@Table(name = "commands")
public class CommandDB {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    private Long id;

    @Column(name = "mode")
    String mode;

    @Column(name = "execCommands")
    @ElementCollection
    List<String> execCommands;

    @Column(name = "waitForExit")
    String waitForExit;

    @ManyToOne
    @JoinColumn(name = "application_id", referencedColumnName = "id")
    ApplicationDB application;

}
