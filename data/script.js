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
  console.log(event.data);
  var myObj = JSON.parse(event.data);
  var keys = Object.keys(myObj);

  for (var i = 0; i < keys.length; i++) {
    var key = keys[i];
    document.getElementById(key).innerHTML = myObj[key];
  } 

  // Plotting stuff
  data.labels.push(myObj.height);
  data.datasets[0].data.push(myObj.volume); 
  myChart.update(); 
  updateTable(); 
}

// Plotting stuff
// Initialize Chart.js
const ctx = document.getElementById("realtimeChart").getContext("2d");
const data = {
  labels: [],
  datasets: [
    {
      label: "Volume vs Height",
      borderColor: "rgb(75, 192, 192)",
      backgroundColor: "rgba(75, 192, 192, 0.2)",
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
  const dataTableBody = document.getElementById('dataTableBody');
  const latestData = data.labels.map((label, index) => ({
      height: label,
      volume: data.datasets[0].data[index],
  }));

  // Clear existing rows
  dataTableBody.innerHTML = '';

  // Append new rows
  latestData.forEach((rowData) => {
      const row = document.createElement('tr');
      Object.values(rowData).forEach((value) => {
          const cell = document.createElement('td');
          cell.textContent = value;
          row.appendChild(cell);
      });
      dataTableBody.appendChild(row);
  });
}


// // Plot simulation 
// let currentHeight = 0;
// setInterval(() => {

//   if(currentHeight > 40 ) {
//     return;
//   }

//   // Generate random data for simulation
//   const newData = {
//       height: currentHeight, 
//       volume: Math.ceil(Math.exp(currentHeight / 10)),
//   };

//   // Update chart data
//   data.labels.push(newData.height);
//   data.datasets[0].data.push(newData.volume);

//   // Update the chart
//   myChart.update(); 
//   updateTable(); 

//   currentHeight += 1;
// }, 500); // Adjust the interval as needed

