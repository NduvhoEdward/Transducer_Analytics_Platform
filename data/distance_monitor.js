//
let distance = document.getElementById("distance");

let websocket = null;
import WebSocketModule from "/WebSocketModule.js";
window.addEventListener("load", function () {
  WebSocketModule.initWebSocket();
  websocket = WebSocketModule.getWebSocket();

  // Subscribe to WebSocket events
  WebSocketModule.subscribe(function (data_from_mcu) {
    console.log(data_from_mcu);
    distance.textContent = data_from_mcu.height;
  });
});
