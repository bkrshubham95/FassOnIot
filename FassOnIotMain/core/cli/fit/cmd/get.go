/*
Copyright © 2023 NAME HERE <EMAIL ADDRESS>
*/
package cmd

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"net/http"

	"github.com/spf13/cobra"
)

func getApplications(targetUrl string) (*http.Response, error) {
	response, err := http.Get(targetUrl)
	if err != nil {
		return nil, err
	}
	return response, nil
}

// getCmd represents the get command
var getCmd = &cobra.Command{
	Use:   "get",
	Short: "Fetch application objects from the API server",
	Long:  `Makes an API call to the API Server to fetch all application objects as JSON`,
	Run: func(cmd *cobra.Command, args []string) {
		requestURL := fmt.Sprintf("http://%s:%s/get", hostname, port)
		response, err := getApplications(requestURL)

		if err != nil {
			fmt.Printf("Get request failed: %s\n", err)
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
	rootCmd.AddCommand(getCmd)
}
