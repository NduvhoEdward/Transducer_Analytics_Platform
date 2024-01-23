// Sections inherited from the styles.css file, which got its
// section hierarchy from the index.html file.

// Section 1.2.1 -- Cards >> MCU Buttons
let recording = true;
let websocket = WebSocketModule.getWebSocket();

document.getElementById("start-stop").addEventListener("click", function () {
  recording = !recording;
  sendMessage("start_stop");
});
document.getElementById("clear").addEventListener("click", function () {
  clearTable();
  clearGraph(height_vol_chart);
  sendMessage("clear");
});
document.getElementById("reset").addEventListener("click", function () {
  clearTable();
  clearGraph(height_vol_chart);
  sendMessage("reset");
});
document.getElementById("zero").addEventListener("click", function () {
  sendMessage("zero");
});
function sendMessage(message) {
  // if (websocket.readyState === WebSocket.OPEN) {
  websocket.send(message);
  console.log("Sent message:", message);
  // } else {
  // console.error("WebSocket not open. Unable to send message.");
  // }
}

// Section 1.2.2 -- Modal
let cardID = "";
function openModal(cardTitle) {
  let modal = document.getElementById("myModal");
  modal.style.display = "block";
  let modal_name = document
    .getElementById(cardTitle)
    .parentNode.parentNode.querySelector(".card-title").textContent;
  let modal_units = document
    .getElementById(cardTitle)
    .parentNode.parentNode.querySelector(".reading").textContent;
  let modal_title = `${modal_name} (${modal_units.trim()})`;
  document.getElementById("modal-input-title").textContent = modal_title;

  cardID = cardTitle;
}
function closeModal() {
  document.getElementById("myModal").style.display = "none";
}
function sendCardData() {
  let inputValue = document.getElementById("inputValue").value;
  if (inputValue === "") {
    closeModal();
    return;
  }
  websocket.send(`${cardID}:${inputValue}`);
  console.log(`Sending data: ${cardID}:${inputValue}`);
  document.getElementById("inputValue").value = "";
  closeModal();
}

// Section 1.2.3 -- Chart
const ctx = document.getElementById("realtimeChart").getContext("2d");
const height_vol_data = {
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
  data: height_vol_data,
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
const height_vol_chart = new Chart(ctx, config);
function clearGraph(chart) {
  chart.data.labels = [];
  chart.data.datasets.forEach((dataset) => {
    dataset.data = [];
  });
  chart.update(); // Update the chart to reflect the changes
}

// Section 1.2.4 -- Raw Data >> Table
function updateTable() {
  const dataTableBody = document.getElementById("dataTableBody");
  const latestData = height_vol_data.labels.map((label, index) => ({
    height: label,
    volume: height_vol_data.datasets[0].data[index],
  }));

  // Clear existing rows
  // dataTableBody.innerHTML = "";

  // Append new rows
  // latestData.forEach((rowData) => {
  //   const row = document.createElement("tr");
  //   Object.values(rowData).forEach((value) => {
  //     const cell = document.createElement("td");
  //     cell.textContent = value;
  //     row.appendChild(cell);
  //   });
  //   dataTableBody.appendChild(row);
  // });

  //

  // Clear existing rows
  dataTableBody.innerHTML = "";
  // Initialize an empty string to store the HTML
  let tableHTML = "";
  // Iterate through the arrays and concatenate HTML
  for (let i = 0; i < height_vol_data.labels.length; i++) {
    // Create HTML for a row and append it to the string
    tableHTML += `<tr><td>${height_vol_data.labels[i]}</td><td>${height_vol_data.datasets[0].data[i]}</td></tr>`;
  }
  dataTableBody.innerHTML = tableHTML;
}

//

function clearTable() {
  const tableBody = document.getElementById("dataTableBody");
  tableBody.innerHTML = "";
}

// Section 1.4 -- Lower Buttons
function fitPolynomial() {
  // Perform polynomial regression
  const degree = 2;
  const result = regression.polynomial(
    height_vol_data.labels.map((label, index) => [
      label,
      height_vol_data.datasets[0].data[index],
    ]),
    { order: degree }
  );
  const coefficients = result.equation;
  coefficients.reverse();

  // Display the obtained polynomial equation
  const equationText = `Equation: ${coefficients
    .map((coefficient, index) => `${coefficient.toFixed(2)} * x^${index}`)
    .join(" + ")}`;

  // Calculate y-values based on the equation
  const fittedData = height_vol_data.labels.map((label) => {
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
  if (height_vol_chart.data.datasets.length > 1) {
    // Remove the previous fitted curve dataset
    height_vol_chart.data.datasets.pop();
  }

  height_vol_chart.data.datasets.push({
    label: "Fitted Curve",
    borderColor: "rgba(255, 0, 0, 1)",
    backgroundColor: "rgba(255, 0, 0, 0.2)",
    data: fittedData,
    fill: true,
  });

  height_vol_chart.update();
}
const fitButton = document.getElementById("fitButton");
fitButton.addEventListener("click", fitPolynomial);

function convertToCSV(height_vol_data, metadata) {
  const mainHeader = ["Height", "Volume"];
  const mainRows = height_vol_data.labels.map((label, index) => [
    label,
    data.datasets[0].data[index],
  ]);

  const mainCSV = [mainHeader, ...mainRows]
    .map((row) => row.join(","))
    .join("\n");
  const csvContent = `# Metadata: ${JSON.stringify(metadata)}\n${mainCSV}`;

  return csvContent;
}
function downloadCSV() {
  const currentDate = new Date();
  const formattedDateTime = currentDate.toISOString();
  const metadata = {
    Time: formattedDateTime,
    Density: document.getElementById("density").textContent,
    Max_Height: document.getElementById("max_height").textContent,
    K_Factor: document.getElementById("k_factor").textContent,
    Sampling_Rate: document.getElementById("sampling_rate").textContent,
  };
  const csvContent = convertToCSV(height_vol_data, metadata);
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

//
//
// Section Z -- Mainly for the MCU stuff
import WebSocketModule from "/WebSocketModule.js";
import WebSocketSimulator from "./WebSocketSimulator.js";

window.addEventListener("load", function () {
  WebSocketModule.initWebSocket();
  websocket = WebSocketModule.getWebSocket();

  // Subscribe to real WebSocket events
  WebSocketModule.subscribe(function (data_from_mcu) {
    handle_pressure_data(data_from_mcu);
  });
});

function handle_pressure_data(data_from_mcu) {
  let keys = Object.keys(data_from_mcu);

  for (let i = 0; i < keys.length; i++) {
    let key = keys[i];

    if (key === "height" || key === "volume") {
      continue;
    }
    document.getElementById(key).innerHTML =
      data_from_mcu[key][data_from_mcu[key].length - 1];
  }

  if (recording) {
    height_vol_data.labels.push(...data_from_mcu.current_height);
    height_vol_data.datasets[0].data.push(...data_from_mcu.volume);

    height_vol_chart.update();
    updateTable();
  }
}
