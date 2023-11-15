package com.example.demo.Model;

import jakarta.persistence.*;
import lombok.*;
import org.hibernate.annotations.CreationTimestamp;
import org.hibernate.annotations.UpdateTimestamp;

import java.sql.Date;
import java.util.List;

@Getter
@Setter
@Builder
@Entity
@Table(name = "applications")
@NoArgsConstructor
@AllArgsConstructor
public class ApplicationDB {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    private Long id;

    @Column(name = "name")
    String name;

    @Column(name = "version")
    String version;
    @Column(name = "runtimeEnvironment")
    String runtimeEnvironment;

    @Column(name = "requiredPlatform")
    String requiredPlatform;

    @Column(name = "isContainerized")
    boolean isContainerized;

    @Column(name = "dependencies")
    String dependencies;

    @Column(name = "subsTopic")
    @ElementCollection
    List<String> subsTopic;

    @Column(name = "pubsTopic")
    @ElementCollection
    List<String> pubsTopic;

    @OneToMany(mappedBy = "application")
    List<CommandDB> commandDBS;

    @Column(name = "status")
    String status;

    @Column(name = "deploymentEngineReportedStatus")
    String deploymentEngineReportedStatus;

    @Column(name = "isProcessed")
    Boolean isProcessed;

    @Column(name = "created")
    @Temporal(TemporalType.DATE)
    @CreationTimestamp
    Date created;

    @Column(name = "update")
    @Temporal(TemporalType.DATE)
    @UpdateTimestamp
    Date updated;

    @Column(name = "extractedPath")
    String extractedPath;
}

// Shubham's Class.

//class Schema(Base):
//
//        _tablename_ = "schema"
//
//
//        id =  Column(Integer, primary_key = True)
//        name    =  Column(String(30))
//        version    =  Column(String(30))
//
//
//
//
//        runtimeEnvironment     =  Column(String(30))
//        requiredPlatform     =  Column(String(30))
//        run_containerized  =  Column(Boolean, unique=False, default=True)
//
//        dependecies     =  Column(String(30))
//
//        subsTopic     =  Column(String(30))
//
//        Pubs_Topic     =  Column(String(30))
//
//
//        mode     =  Column(String(30))
//        execCommands     =  Column(String(30))
//
//        waitForExit     =  Column(String(30))
//
//
//        status     =  Column(String(30))
//
//        deInternalStatus     =  Column(String(30))
//
//        process = relationship('Process', back_populates='schema')
//
//        stats = relationship('Stats', back_populates='stats')
//
//
//
//
//
//
//class Process(Base):
//        _tablename_ = "process"
//
//        id   =  Column(Integer, primary_key = True)
//        cmd     =  Column(String(30))
//        status     =  Column(String(30))
//        statusInfo     =  Column(String(30))
//
//        process_id = Column(Integer, ForeignKey('schema.id'))
//
//
//        schema = relationship('Schema', back_populates='process')
//
//
//class Stats(Base):
//        _tablename_ = "stats"
//
//        id  =  Column(Integer, primary_key=True)
//        msgPublished  =  Column(Integer)
//        msgSentToOpp   =  Column(Integer)
//        resourceUsage   =  Column(Float)
//
//        stats_id = Column(Integer, ForeignKey('schema.id'))
//
//        stats = relationship('Schema', back_populates='stats')