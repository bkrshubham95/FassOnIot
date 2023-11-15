package com.example.demo.Repository;

import com.example.demo.Model.CommandDB;
import org.springframework.data.jpa.repository.JpaRepository;

public interface CommandRepository extends JpaRepository<CommandDB, Long> {
}
