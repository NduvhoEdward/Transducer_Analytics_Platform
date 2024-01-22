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
    // distance.textContent = data_from_mcu.height;
  });

  // Initialize WebSocket simulator module
  // WebSocketSimulator.useSimulatedWebSocket();
  // WebSocketSimulator.subscribe((data) => {
  //   console.log("Simulated WebSocket Data Received:", data);
  //   distance.textContent = data.height;
  //   // handle_distance_data(data);
  //   add_data(data.density, data.volume, data.test_time);
  // });

  // WebSocketSimulator.useRealWebSocket();
  // WebSocketSimulator.useSimulatedWebSocket();
});

//

var MAX_DATA_SET_LENGTH = 25;

var canvas = document.getElementById("time-chart");
var data = {
  datasets: [
    {
      label: "current1_mA",
      borderColor: "rgb(237, 18, 237)",
      fill: false,
      borderWidth: 3,
    },
    {
      label: "current2_mA",
      borderColor: "rgb(0, 115, 255)",
      fill: false,
      borderWidth: 3,
    },
  ],
};
var options = {
  animation: false,
  scales: {
    yAxes: [
      {
        type: "linear",
      },
    ],

    y: {
      type: "linear",
    },

    x: {
      type: "linear",
      position: "bottom",
      scaleLabel: {
        display: true,
        labelString: "Height",
      },
    },
  },
  showLines: true,
};
var time_chart = new Chart.Line(canvas, {
  data: data,
  options: options,
});

function add_data(current1 = [], current2 = [], test_time = []) {
  var datasets = time_chart.data.datasets;
  var labels = time_chart.data.labels;
  var current1DataSet = datasets[0].data;
  var current2DataSet = datasets[1].data;

  var current1DataLength = current1DataSet.length;

  if (current1DataLength > MAX_DATA_SET_LENGTH) {
    current1DataSet.splice(0, current1.length);
    current2DataSet.splice(0, current2.length);
    labels.splice(0, test_time.length);
  }

  Array.prototype.push.apply(labels, test_time);
  Array.prototype.push.apply(current1DataSet, current1);
  Array.prototype.push.apply(current2DataSet, current2);

  time_chart.update();
}
