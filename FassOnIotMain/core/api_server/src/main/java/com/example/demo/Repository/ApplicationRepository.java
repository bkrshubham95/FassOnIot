package com.example.demo.Repository;


import com.example.demo.Model.ApplicationDB;
import org.springframework.data.jpa.repository.JpaRepository;

public interface ApplicationRepository extends JpaRepository<ApplicationDB,Long> {

//    //custom methods
//    List<Schema> findByPublished(boolean published);
//    List<Schema> hello(String name);
}
