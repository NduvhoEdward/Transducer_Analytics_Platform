//
import ProcessCSVData from "./ProcessCSVData.js";
const WebSocketModule = {
  gateway: `ws://${window.location.hostname}/ws`,
  websocket: null,
  subscribers: [],

  initWebSocket: function () {
    console.log("Trying to open a WebSocket connection…");
    this.websocket = new WebSocket(this.gateway);
    this.websocket.onopen = this.onOpen.bind(this);
    this.websocket.onclose = this.onClose.bind(this);
    this.websocket.onmessage = this.onMessage.bind(this);
  },

  subscribe: function (callback) {
    this.subscribers.push(callback);
  },

  notifySubscribers: function (data) {
    this.subscribers.forEach((callback) => callback(data));
  },

  onOpen: function (event) {
    console.log("Connection opened");
    this.getReadings();
  },

  getReadings: function () {
    this.websocket.send("getReadings");
  },

  onClose: function (event) {
    console.log("Connection closed");
    setTimeout(this.initWebSocket.bind(this), 2000);
  },
  onMessage: function (event) {
    console.log(event.data);
    let data_from_mcu = ProcessCSVData(event.data);
    this.notifySubscribers(data_from_mcu);
  },

  getWebSocket: function () {
    return this.websocket;
  },
};

export default WebSocketModule;
