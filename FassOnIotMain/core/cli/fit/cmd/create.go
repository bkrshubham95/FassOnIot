/*
Copyright © 2023 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"bytes"
	"encoding/json"
	"fmt"
	"github.com/spf13/cobra"
	"io"
	"mime/multipart"
	"net/http"
	"os"
	"path/filepath"
)

var archivePath string

func postFile(filename string, targetUrl string) (*http.Response, error) {
	body := new(bytes.Buffer)
	writer := multipart.NewWriter(body)
	part, err := writer.CreateFormFile("inputZipFile", filepath.Base(filename))
	if err != nil {
		return nil, err
	}
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()
	_, err = io.Copy(part, file)
	if err != nil {
		return nil, err
	}
	err = writer.Close()
	if err != nil {
		return nil, err
	}
	request, err := http.NewRequest("POST", targetUrl, body)
	request.Header.Set("Content-Type", writer.FormDataContentType())
	client := &http.Client{}
	response, err := client.Do(request)
	if err != nil {
		return nil, err
	}
	return response, nil
}

// createCmd represents the create command
var createCmd = &cobra.Command{
	Use:   "create",
	Short: "Create and initiate deployment of an application",
	Long: `Create allows you to deploy an application.
The application code and configuration is provided through a zip archive. 
The application configuration must be located in the root of the archive.`,
	Run: func(cmd *cobra.Command, args []string) {
		requestURL := fmt.Sprintf("http://%s:%s/create", hostname, port)
		response, err := postFile(archivePath, requestURL)
		if err != nil {
			fmt.Printf("Create request failed: %s\n", err)
			return
		}
		responseBody, err := io.ReadAll(response.Body)
		if err != nil {
			fmt.Printf("Could not process response body: %s\n", err)
			return
		}

		jsonResponse := &bytes.Buffer{}
		err = json.Indent(jsonResponse, responseBody, "", "   ")
		if err != nil {
			fmt.Printf("Could not parse response JSON: %s\n", err)
			return
		}
		if response.StatusCode != 200 {
			fmt.Printf("Server returned error: %s\n", jsonResponse)
		} else {
			fmt.Printf("Request successful:\n%s\n", jsonResponse)
		}

		if response != nil {
			defer response.Body.Close()
		}
	},
}

func init() {

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	createCmd.PersistentFlags().StringVarP(&archivePath, "archive", "a", "", "Path to the application archive")
	createCmd.MarkPersistentFlagRequired("archive")
	rootCmd.AddCommand(createCmd)
	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// createCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}
