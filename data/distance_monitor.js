//
let distance = document.getElementById("distance");

let websocket = null;
import WebSocketModule from "/WebSocketModule.js";
import WebSocketSimulator from "./WebSocketSimulator.js";

window.addEventListener("load", function () {
  WebSocketModule.initWebSocket();
  websocket = WebSocketModule.getWebSocket();

  // Subscribe to real WebSocket events
  WebSocketModule.subscribe(function (data_from_mcu) {
    console.log(data_from_mcu);
    distance.textContent = data_from_mcu.height;
  });

  // Initialize WebSocket simulator module
  WebSocketSimulator.useSimulatedWebSocket();
  WebSocketSimulator.subscribe((data) => {
    console.log("Simulated WebSocket Data Received:", data);
    distance.textContent = data.height;
    // handle_distance_data(data);
    add_data(data.density, data.volume, data.time);
  });

  // WebSocketSimulator.useRealWebSocket();
  WebSocketSimulator.useSimulatedWebSocket();
});

//

var MAX_DATA_SET_LENGTH = 15;

var canvas = document.getElementById("time-chart");
var data = {
  datasets: [
    {
      label: "current1",
      borderColor: "rgb(237, 18, 237)",
      fill: false,
      borderWidth: 3,
    },
    {
      label: "current2",
      borderColor: "rgb(0, 115, 255)",
      fill: false,
      borderWidth: 3,
    },
  ],
};
var options = {
  scales: {
    yAxes: [
      {
        type: "linear",
      },
    ],

    xAxes: [
      {
        type: "time",
        ticks: {
          maxTicksLimit: 5,
        },
        time: {
          unit: "second",
          displayFormats: {
            second: "HH:mm:ss",
          },
          tooltipFormat: "HH:mm:ss",
        },
      },
    ],
  },
  showLines: true,
};
var time_chart = new Chart.Line(canvas, {
  data: data,
  options: options,
});

function add_data(current1 = NaN, current2 = NaN, label = moment()) {
  var datasets = time_chart.data.datasets;
  var labels = time_chart.data.labels;
  var current1DataSet = datasets[0].data;
  var current2DataSet = datasets[1].data;

  var current1DataLength = current1DataSet.length;
  var current2DataLength = current2DataSet.length;

  // if upload/download's data set has more than MAX_DATA_SET_LENGTH entries,
  // remove the first one entry and push on a new data entry
  var didRemoveData = false;
  if (current1DataLength > MAX_DATA_SET_LENGTH) {
    current1DataSet.shift();
    didRemoveData = true;
  }

  if (current2DataLength > MAX_DATA_SET_LENGTH) {
    current2DataSet.shift();
    didRemoveData = true;
  }

  // if either download or upload data was removed, we also need to remove
  // the first label to keep the data from squeezing in.
  if (didRemoveData) {
    labels.shift();
  }

  labels.push(label);
  current1DataSet.push(current1);
  current2DataSet.push(current2);
  time_chart.update();
}

// setInterval(reLoad, 1000);
