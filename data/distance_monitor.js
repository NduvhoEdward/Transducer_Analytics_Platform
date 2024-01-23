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
    add_data(
      data_from_mcu.density,
      data_from_mcu.volume,
      data_from_mcu.test_time
    );
  });
});

//

let MAX_DATA_SET_LENGTH = 25;

let canvas = document.getElementById("time-chart");
let data = {
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
let options = {
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
let time_chart = new Chart.Line(canvas, {
  data: data,
  options: options,
});

function add_data(current1 = [], current2 = [], test_time = []) {
  let datasets = time_chart.data.datasets;
  let labels = time_chart.data.labels;
  let current1DataSet = datasets[0].data;
  let current2DataSet = datasets[1].data;

  let current1DataLength = current1DataSet.length;

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
