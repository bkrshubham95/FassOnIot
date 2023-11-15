package com.example.demo.Utils;

import java.io.File;
import java.util.logging.Logger;

public class Utils {
    static Logger logger = Logger.getLogger(Utils.class.getName());
    public static void ensureWorkspaceDir() {
        String workspaceDirPath = ".api_server";
        File workspaceDir = new File(workspaceDirPath);
        if(! workspaceDir.exists()) {
            if(workspaceDir.mkdirs()) {
                logger.info("Created .api_server");
            }
        }
    }
}
