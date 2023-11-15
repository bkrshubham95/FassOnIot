const uploadForm = document.getElementById('upload-form');
const createResponseDiv = document.getElementById('createResponse');
const getResponseDiv = document.getElementById('getResponse');
const baseURL = "http://" + location.hostname + ":8082";
var newApp = "";

uploadForm.addEventListener('submit', (event) => {
	event.preventDefault();
	const fileInput = document.querySelector('input[type="file"]');
	const file = fileInput.files[0];
	const formData = new FormData();
	formData.append('inputZipFile', file);

	fetch(baseURL + '/create', {
		method: 'POST',
		body: formData
	}).then((response) => {
		if (response.ok) {
			response.json().then((data) => {
				newApp = data.name;
			});
		} else {
			createResponseDiv.textContent = 'Upload failed';
		}
	}).catch((error) => {
		createResponseDiv.textContent = 'An error occurred';
	});
});

function populateTable(data) {
	var tbody = document.querySelector('#statusTable tbody');
	tbody.innerHTML = "";
    for(var i = 0; i < data.length; i++) {
      	var tr = document.createElement('tr');
      	tr.innerHTML =  '<td>' + data[i].name + '</td>' +
						'<td>' + data[i].status + '</td>' +
						'<td>' + data[i].deploymentEngineReportedStatus + '</td>';
      	tbody.appendChild(tr);
		if(data[i].name === newApp) {
			updateStatus(data[i].deploymentEngineReportedStatus)
		}
    }
}

function poll() {
	fetch(baseURL + '/get').then((response) => {
		if (response.ok) {
			response.json().then((data) => {
				populateTable(data);
				setTimeout(poll, 1000);
			});
		} else {
			getResponseDiv.textContent = 'An error occurred';
		}
	}).catch((error) => {
		getResponseDiv.textContent = 'An error occurred';
	});
}
poll();

function showHistory(id) {
	var x = document.getElementById(id);
	if (x.className.indexOf("w3-show") == -1) {
	  x.className += " w3-show";
	  x.previousElementSibling.className = 
	  x.previousElementSibling.className.replace("w3-black", "w3-grey");
	} else { 
	  x.className = x.className.replace(" w3-show", "");
	  x.previousElementSibling.className = 
	  x.previousElementSibling.className.replace("w3-grey", "w3-black");
	}
}

function updateStatus(status) {
	console.log(status)
	var header = document.getElementById('newAppName');
	header.innerText = "Deploying: " + newApp;
	if(status.toUpperCase() === "Commands Executed".toUpperCase()) {
		header.innerText = "Latest deployed: " + newApp;
	}
	const statusValues = ["Directory Structure Created", "Artifacts Transferred", "Pushed Pub Sub Info To Data XChange", "Commands Executed"]
	var tr = document.createElement("tr");
	var isGreen = true;
	var cellColor = "w3-green";

	if(status.toUpperCase() == "NOT AVAILABLE"){
		isGreen = false;
	}

	for(var i = 0; i < statusValues.length; i++) {
		var val = '<td class="w3-green">' + statusValues[i] + '</td>'
		if(isGreen === false){
			val = '<td align="center"><img src="loading.gif" style="width:100%;margin: 0 auto;"/></td>'
		}
		if(statusValues[i].toUpperCase() === status.toUpperCase()) {
			isGreen = false;
		}
		tr.innerHTML = tr.innerHTML + val;
	}
	var tableStatus = document.getElementById("newAppStatus")
	tableStatus.innerHTML = '';
	tableStatus.appendChild(tr);
}
