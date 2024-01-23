function ProcessCSVData(csv_data) {
  // Arrays to store column values
  let currentHeightValues = [];
  let totalVolumeValues = [];
  let densityValues = [];
  let kFactorValues = [];
  let zeroHeightValues = [];
  let maxHeightValues = [];
  let samplingRateValues = [];

  // Split CSV chunk into an array of rows
  let rows = csv_data.trim().split("\n");

  // Process each row
  rows.forEach(function (row) {
    // Split row into an array of values
    let values = row.split(",");

    let currentHeight = parseFloat(values[0]);
    let totalVolume = parseFloat(values[1]);
    let density = parseFloat(values[2]);
    let kFactor = parseFloat(values[3]);
    let zeroHeight = parseFloat(values[4]);
    let maxHeight = parseFloat(values[5]);
    let samplingRate = parseFloat(values[6]);

    currentHeightValues.push(currentHeight);
    totalVolumeValues.push(totalVolume);
    densityValues.push(density);
    kFactorValues.push(kFactor);
    zeroHeightValues.push(zeroHeight);
    maxHeightValues.push(maxHeight);
    samplingRateValues.push(samplingRate);
  });

  return {
    current_height: currentHeightValues,
    volume: totalVolumeValues,
    density: densityValues,
    k_factor: kFactorValues,
    zero_height: zeroHeightValues,
    max_height: maxHeightValues,
    sampling_rate: samplingRateValues,
  };
}

export default ProcessCSVData;
