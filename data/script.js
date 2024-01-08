// import { PolynomialRegression } from 'ml-regression-polynomial';

var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
// Init web socket when the page loads
window.addEventListener("load", onload);

function onload(event) {
  initWebSocket();
}

function getReadings() {
  websocket.send("getReadings");
}

function initWebSocket() {
  console.log("Trying to open a WebSocket connection…");
  websocket = new WebSocket(gateway);
  websocket.onopen = onOpen;
  websocket.onclose = onClose;
  websocket.onmessage = onMessage;
}

// When websocket is established, call the getReadings() function
function onOpen(event) {
  console.log("Connection opened");
  getReadings();
}

function onClose(event) {
  console.log("Connection closed");
  setTimeout(initWebSocket, 2000);
}

// Function that receives the message from the ESP32 with the readings
function onMessage(event) {
  // console.log(event.data);
  var data_from_mcu = JSON.parse(event.data);
  var keys = Object.keys(data_from_mcu);

  for (var i = 0; i < keys.length; i++) {
    var key = keys[i];

    if (key === "height" || key === "volume") {
      continue;
    }
    document.getElementById(key).innerHTML = data_from_mcu[key];
  }

  // Plotting stuff
  data.labels.push(data_from_mcu.height);
  data.datasets[0].data.push(data_from_mcu.volume);
  myChart.update();
  updateTable();
}

document.getElementById("start-stop").addEventListener("click", function () {
  sendMessage("start_stop");
});
document.getElementById("clear").addEventListener("click", function () {
  clearTable(); 
  clearGraph(myChart); 
  sendMessage("clear");
});
document.getElementById("reset").addEventListener("click", function() {
  clearTable(); 
  clearGraph(myChart); 
  sendMessage('reset');
});

function sendMessage(message) {
  if (websocket.readyState === WebSocket.OPEN) {
    websocket.send(message);
    console.log("Sent message:", message);
  } else {
    console.error("WebSocket not open. Unable to send message.");
  }
}

// Plotting stuff
const ctx = document.getElementById("realtimeChart").getContext("2d");
const data = {
  labels: [],
  datasets: [
    {
      label: "Volume vs Height",
      borderColor: "#f5911e",
      backgroundColor: "002841",
      data: [],
    },
  ],
};

const config = {
  type: "line",
  data: data,
  options: {
    // animation: false,
    scales: {
      x: {
        type: "linear",
        position: "bottom",
        scaleLabel: {
          display: true,
          labelString: "Height",
        },
      },
      y: {
        scaleLabel: {
          display: true,
          labelString: "Volume",
        },
        min: 0,
      },
    },
  },
};

const myChart = new Chart(ctx, config);

// Update table function
function updateTable() {
  const dataTableBody = document.getElementById("dataTableBody");
  const latestData = data.labels.map((label, index) => ({
    height: label,
    volume: data.datasets[0].data[index],
  }));

  // Clear existing rows
  dataTableBody.innerHTML = "";

  // Append new rows
  latestData.forEach((rowData) => {
    const row = document.createElement("tr");
    Object.values(rowData).forEach((value) => {
      const cell = document.createElement("td");
      cell.textContent = value;
      row.appendChild(cell);
    });
    dataTableBody.appendChild(row);
  });
}

// Function to clear the table
function clearTable() {
  const tableBody = document.getElementById("dataTableBody");
  tableBody.innerHTML = ""; 
}

// Function to clear the graph 
function clearGraph(chart) {
  chart.data.labels = [];
  chart.data.datasets.forEach((dataset) => {
      dataset.data = [];
  });
  chart.update(); // Update the chart to reflect the changes
}
// Simulation
// let currentHeight = 0;
// setInterval(() => {
//   var beenDone = 0;
//   if (currentHeight > 50) {

//     if(beenDone == 1){
//       return;
//     }
//     // Log the data points before regression
//     const labels = data.labels.slice(1);
//     const datasetData = data.datasets[0].data.slice(1);

//     const pointsForRegression = labels.map((label, index) => [
//       label,
//       datasetData[index],
//     ]);

//     const v = labels.map((label, index) => [label, data.datasets[0].data[index]]);

//     const degree = 2;
//     const result = regression.polynomial(
//       data.labels.map((label, index) => [label, data.datasets[0].data[index]]),
//       { order: degree }
//     );

//     const coefficients = result.equation;
//     coefficients.reverse();

//     // Display the obtained polynomial equation
//     const equationText = `Equation: ${coefficients
//       .map((coefficient, index) => `${coefficient.toFixed(2)} * x^${index}`)
//       .join(" + ")}`;
//     // console.log(`Polynomial Coefficients (Degree ${degree}):`, coefficients);
//     // console.log(equationText);

//     // Display the equation on the HTML page (assuming you have an element with id 'equationText' in your HTML)
//     const equationElement = document.getElementById("equationText");
//     equationElement.textContent = equationText;
//     beenDone = 1;
//     return;
//   }

//   // Generate random data for simulation
//   const newData = {
//     height: currentHeight,
//     volume: Math.ceil(Math.exp(currentHeight / 10)), 
//   };

//   // Update chart data
//   data.labels.push(newData.height);
//   data.datasets[0].data.push(newData.volume);

//   // Update the chart
//   myChart.update();
//   updateTable();
//   currentHeight += 1;
// }, 250); // Adjust the interval as needed

// Function to convert data to CSV format
function convertToCSV(data) {
  const header = ["Height", "Volume"];
  const rows = data.labels.map((label, index) => [
    label,
    data.datasets[0].data[index],
  ]);
  const allRows = [header, ...rows];
  return allRows.map((row) => row.join(",")).join("\n");
}

// Function to download CSV file
function downloadCSV() {
  const csvContent = convertToCSV(data);
  const blob = new Blob([csvContent], { type: "text/csv" });
  const link = document.createElement("a");
  link.href = window.URL.createObjectURL(blob);
  link.download = "PressurePrecisionProfiler_data.csv";
  document.body.appendChild(link);
  link.click();
  document.body.removeChild(link);
}

const exportButton = document.getElementById("exportButton");
exportButton.addEventListener("click", downloadCSV);

// Fitting stuff 

// Function to perform polynomial regression and update the chart
function fitPolynomial() {
  // Perform polynomial regression
  const degree = 2;
  const result = regression.polynomial(
    data.labels.map((label, index) => [label, data.datasets[0].data[index]]),
    { order: degree }
  );
  const coefficients = result.equation;
  coefficients.reverse();

  // Display the obtained polynomial equation
  const equationText = `Equation: ${coefficients
    .map((coefficient, index) => `${coefficient.toFixed(2)} * x^${index}`)
    .join(" + ")}`;

  // Calculate y-values based on the equation
  const fittedData = data.labels.map((label) => {
    const x = label;
    const y = coefficients.reduce(
      (acc, coefficient, index) => acc + coefficient * Math.pow(x, index),
      0
    );
    return y;
  });

  // Display the equation on the HTML page
  const equationElement = document.getElementById("equationText");
  equationElement.textContent = equationText;

  // Update the chart with the fitted data
  if (myChart.data.datasets.length > 1) {
    // Remove the previous fitted curve dataset
    myChart.data.datasets.pop();
  }

  myChart.data.datasets.push({
    label: "Fitted Curve",
    borderColor: "rgba(255, 0, 0, 1)",
    backgroundColor: "rgba(255, 0, 0, 0.2)",
    data: fittedData,
    fill: true,
  });

  myChart.update();
}

const fitButton = document.getElementById("fitButton");
fitButton.addEventListener("click", fitPolynomial);




var cardID = '';
function openModal(cardTitle) {
  var modal = document.getElementById('myModal');
  modal.style.display = 'block'; 
  var modal_name = document.getElementById(cardTitle).parentNode.parentNode.querySelector('.card-title').textContent;
  var modal_units = document.getElementById(cardTitle).parentNode.parentNode.querySelector('.reading').textContent;
  var modal_title = `${modal_name} (${modal_units.trim()})`;
  document.getElementById('modal-input-title').textContent = modal_title; 

  cardID = cardTitle;
}

function closeModal() {
  document.getElementById('myModal').style.display = 'none';
}

function sendCardData() {
  var inputValue = document.getElementById('inputValue').value; 
  if (inputValue === '') {
    closeModal();
    return;
  websocket.send(`${cardID}:${inputValue}`);
  console.log(`Sending data: ${cardID}:${inputValue}`);
  document.getElementById('inputValue').value = ''; 
  closeModal();
}