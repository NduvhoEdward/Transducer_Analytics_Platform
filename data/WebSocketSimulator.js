const WebSocketSimulator = {
  subscribers: [],
  useSimulation: true,

  useRealWebSocket: function () {
    this.useSimulation = false;
  },

  useSimulatedWebSocket: function () {
    this.useSimulation = true;
    this.simulateData();
  },

  subscribe: function (callback) {
    this.subscribers.push(callback);
  },

  notifySubscribers: function (data) {
    this.subscribers.forEach((callback) => callback(data));
  },

  simulateData: function () {
    if (this.useSimulation) {
      setInterval(() => {
        const simulatedData = {
          height: getRandomFloat(0, 10, 3),
          volume: getRandomFloat(0, 100, 3),
          density: getRandomFloat(0, 5, 2),
          k_factor: getRandomFloat(0, 10, 2),
          current_height: getRandomFloat(0, 10, 3),
          zero_height: getRandomFloat(0, 10, 3),
          max_height: getRandomFloat(0, 20, 3),
          sampling_rate: getRandomFloat(1, 10, 2),
        };

        this.notifySubscribers(simulatedData);
      }, 1000);
    }
  },
};

function getRandomFloat(min, max, precision) {
  const value = Math.random() * (max - min) + min;
  return parseFloat(value.toFixed(precision));
}

export default WebSocketSimulator;
