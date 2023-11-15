package com.example.demo.Controllers;

import com.example.demo.Model.ApplicationDB;
import com.example.demo.Model.Application;
import com.example.demo.Model.Command;
import com.example.demo.Model.CommandDB;
import com.example.demo.Repository.ApplicationRepository;
import com.example.demo.Repository.CommandRepository;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.google.gson.Gson;
import net.lingala.zip4j.ZipFile;
import net.lingala.zip4j.exception.ZipException;
import org.apache.tomcat.util.http.fileupload.IOUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.multipart.MultipartFile;
import org.springframework.web.server.ResponseStatusException;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.*;
import java.util.logging.Logger;

import static com.example.demo.Utils.Utils.ensureWorkspaceDir;

@RestController
public class APIController {

    @Autowired
    ApplicationRepository applicationRepository;

    @Autowired
    CommandRepository commandRepository;

    static Logger logger = Logger.getLogger(APIController.class.getName());

    @CrossOrigin 
    @PostMapping(value = "/create", produces = "application/json")
    public String create(@RequestParam("inputZipFile") MultipartFile inputZipFile) throws IOException {


        /**
         * save file to temp
         */
        File zip = File.createTempFile(UUID.randomUUID().toString(), "temp");
        FileOutputStream o = new FileOutputStream(zip);
        IOUtils.copy(inputZipFile.getInputStream(), o);
        o.close();

        /**
         * unizp file from temp by zip4j
         */
        ensureWorkspaceDir();
        String destinationPath = ".api_server/" + UUID.randomUUID().toString();
        File destinationDir = new File(destinationPath);
        destinationDir.mkdirs();
        ApplicationDB applicationDB = null;
        try {
            ZipFile zipfile = new ZipFile(zip);
            zipfile.extractAll(destinationPath);
            if(zipfile.getFileHeader("app.config") != null && List.of(destinationDir.list()).contains("app.config")) {
                Application application = checkAppConfigSanity(destinationPath + "/app.config");
                applicationDB = persistToDatabase(application, destinationDir.getAbsolutePath());
            } else {
                throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "app.config not found in the archive root");
            }

        } catch (ZipException e) {
            e.printStackTrace();
        } finally {
            /**
             * delete temp file
             */
            zip.delete();
        }
        if(applicationDB != null)
            return toJSON(applicationDB);
        throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "General check succeeded but object did not persist to DB");
    }

    private String toJSON(ApplicationDB applicationDB) throws JsonProcessingException {
        ObjectMapper objectMapper = new ObjectMapper();
        for(CommandDB commandDB: applicationDB.getCommandDBS())
            commandDB.setApplication(null);
        return objectMapper.writeValueAsString(applicationDB);
    }

    @CrossOrigin
    @GetMapping(value = "/get", produces = "application/json")
    public String get() throws IOException {
        ObjectMapper objectMapper = new ObjectMapper();
        List<ApplicationDB> applicationDBList = new ArrayList<>();
        for(ApplicationDB applicationDB: applicationRepository.findAll()) {
            for(CommandDB commandDB: applicationDB.getCommandDBS())
                commandDB.setApplication(null);
            applicationDBList.add(applicationDB);
        }
        return objectMapper.writeValueAsString(applicationDBList);
    }

    private ApplicationDB persistToDatabase(Application application, String extractedPath) {
       for(ApplicationDB applicationDB: applicationRepository.findAll()) {
            if(Objects.equals(applicationDB.getName(), application.getName())) {
                throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "Application with name: " + applicationDB.getName() + " exists");
            }
       }
        ApplicationDB applicationDB = application.toApplicationDB(extractedPath);
        List<CommandDB> commandDBS = new ArrayList<>();
        for(Command command: application.getCommands()) {
            CommandDB commandDB = command.toCommandDB();
            commandDB.setApplication(applicationDB);
            commandDBS.add(commandDB);
        }
        applicationDB.setCommandDBS(commandDBS);
        applicationRepository.save(applicationDB);
        commandRepository.saveAll(commandDBS);
        return applicationDB;
    }

    private Application checkAppConfigSanity(String appConfigPath) throws IOException {
        File appConfig = new File(appConfigPath);
        ObjectMapper mapper = new ObjectMapper();

        Application application = mapper.readValue(appConfig, Application.class);
        System.out.println(application);
        String validationErrors = "";

        if (application.getName().isBlank())
        {
            logger.warning("Application name empty");
            validationErrors += "Application name empty.\n";
        }
        //2. Check if version is present
        if (application.getVersion().isBlank())
        {
            logger.warning("Version name is NUll");
            validationErrors += "Version empty.\n";
        }
        // 3.Check if this application is publishing to a topic or subscribing to a topic

        // 4. Check if user has specified the platform on which the application has to be deployed
        ArrayList<String> supportedPlatforms = new ArrayList<>(List.of(new String[]{"L4", "BeagleBoneBlack", "Linux"}));
        if (!supportedPlatforms.contains(application.getPlatform()))
        {
            logger.warning("Platform unsupported");
            validationErrors += "Unsupported platform.\n";

        }
        // 5, Should it be run in a containerized environment or not
        if (application.getContainerized().isBlank())
        {
            validationErrors += "Did not specify whether to containerize\n";
            logger.warning("Invalid containerize value");
        }
        // 6. User must specify the commands to run thr application
        if (application.getCommands().length == 0)
        {
            validationErrors += "No commands specified.\n";
            logger.warning("Empty commands");
        }
        if(!validationErrors.isBlank())
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, validationErrors);
        else
            logger.info("Configuration validation ok");
        return application;
    }
}
