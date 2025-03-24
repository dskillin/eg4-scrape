# EG4 Monitoring System: getInverterEnergyInfo Endpoint Documentation

This document explains the fields returned by the `getInverterEnergyInfo` API endpoint and how to access it. The response provides detailed energy-related data for the inverter, including daily and cumulative values.

---

## Endpoint Overview

- **URL**: `https://monitor.eg4electronics.com/WManage/api/inverter/getInverterEnergyInfo`
- **Method**: POST
- **Authentication**: Requires a valid session cookie (obtained via the login process).
- **Parameters**: `serialNum` (the serial number of the inverter).

---

## Request

**Headers**:
- `Content-Type: application/x-www-form-urlencoded`
- `X-Requested-With: XMLHttpRequest`
- `Cookie`: Session cookie (obtained after login)

**Body**:
- `serialNum`: The serial number of the inverter.

Example request body:
```
serialNum=1234567890
```

---

## Response Fields

The response is a JSON object containing the following fields:

- `success`: `bool` – Indicates if the request was successful.
- `serialNum`: `string` – The serial number of the inverter.
- `fwCode`: `string` – Firmware code of the inverter.
- `powerRatingText`: `string` – The power rating of the inverter (e.g., "12kW").
- `lost`: `bool` – Indicates if the inverter is offline or unreachable.
- `hasRuntimeData`: `bool` – Indicates if runtime data is available.
- `soc`: `int` – State of charge of the battery (in %).
- `todayYielding`: `int` – Today's solar generation (in 0.1 kWh units).
- `todayYieldingText`: `string` – Today's solar generation in kWh.
- `todayIncomeText`: `string` – Today's income from solar.
- `todaySavingText`: `string` – Today's savings from solar.
- `totalYielding`: `int` – Total solar generation (in 0.1 kWh).
- `totalYieldingText`: `string` – Total solar generation (kWh).
- `totalCo2ReductionText`: `string` – CO2 reduction in kilograms.
- `totalCoalReductionText`: `string` – Coal reduction in kilograms.
- `totalIncomeText`: `string` – Total solar income.
- `totalSavingText`: `string` – Total solar savings.
- `todayDischarging`: `int`, `todayDischargingText`: `string`
- `totalDischarging`: `int`, `totalDischargingText`: `string`
- `todayCharging`: `int`, `todayChargingText`: `string`
- `totalCharging`: `int`, `totalChargingText`: `string`
- `todayImport`: `int`, `todayImportText`: `string`
- `totalImport`: `int`, `totalImportText`: `string`
- `todayExport`: `int`, `todayExportText`: `string`
- `totalExport`: `int`, `totalExportText`: `string`
- `todayUsage`: `int`, `todayUsageText`: `string`
- `totalUsage`: `int`, `totalUsageText`: `string`
- `pvPieChargeTodayRate`, `pvPieExportTodayRate`, `pvPieUsageTodayRate`
- `pvPieChargeTotalRate`, `pvPieExportTotalRate`, `pvPieUsageTotalRate`

---

## Example Response

```json
{
    "success": true,
    "serialNum": "yoursn",
    "fwCode": "fAAB-1E1E",
    "powerRatingText": "12kW",
    "lost": false,
    "hasRuntimeData": true,
    "soc": 100,
    "todayYielding": 65,
    "todayYieldingText": "6.5",
    "todayIncomeText": "--",
    "todaySavingText": "-.00",
    "totalYielding": 5313,
    "totalYieldingText": "531.3",
    "totalCo2ReductionText": "529.71 kG",
    "totalCoalReductionText": "212.52 kG",
    "totalIncomeText": "--",
    "totalSavingText": "-.00",
    "todayDischarging": 2,
    "todayDischargingText": "0.2",
    "totalDischarging": 2519,
    "totalDischargingText": "251.9",
    "todayCharging": 26,
    "todayChargingText": "2.6",
    "totalCharging": 7612,
    "totalChargingText": "761.2",
    "todayImport": 116,
    "todayImportText": "11.6",
    "totalImport": 85464,
    "totalImportText": "8546.4",
    "todayExport": 368,
    "todayExportText": "36.8",
    "totalExport": 61046,
    "totalExportText": "6104.6",
    "todayUsage": 250,
    "todayUsageText": "25",
    "totalUsage": 132247,
    "totalUsageText": "13224.7",
    "pvPieChargeTodayRate": 138,
    "pvPieExportTodayRate": -1430,
    "pvPieUsageTodayRate": 2292,
    "pvPieChargeTotalRate": 300,
    "pvPieExportTotalRate": -8763,
    "pvPieUsageTotalRate": 9463
}
```

---

## Example PHP Code

```php
<?php
$dataUrl = 'https://monitor.eg4electronics.com/WManage/api/inverter/getInverterEnergyInfo';
$cookieFile = __DIR__ . '/cookies.txt';

$config = require __DIR__ . '/config.php';
$serialNum = $config['serialNum'];

$ch = curl_init($dataUrl);
curl_setopt_array($ch, [
    CURLOPT_RETURNTRANSFER => true,
    CURLOPT_POST => true,
    CURLOPT_POSTFIELDS => http_build_query(['serialNum' => $serialNum]),
    CURLOPT_HTTPHEADER => [
        'Content-Type: application/x-www-form-urlencoded',
        'X-Requested-With: XMLHttpRequest'
    ],
    CURLOPT_COOKIEFILE => $cookieFile,
]);
$response = curl_exec($ch);
curl_close($ch);

if ($response) {
    $data = json_decode($response, true);
    if ($data['success']) {
        echo "Today's Usage: " . $data['todayUsageText'] . " kWh\n";
        echo "Today's Export: " . $data['todayExportText'] . " kWh\n";
        echo "Today's Import: " . $data['todayImportText'] . " kWh\n";
        echo "Today's Solar Yield: " . $data['todayYieldingText'] . " kWh\n";
        echo "Today's Battery Charge: " . $data['todayChargingText'] . " kWh\n";
        echo "Today's Battery Discharge: " . $data['todayDischargingText'] . " kWh\n";
        echo "Battery SOC: " . $data['soc'] . "%\n";
    } else {
        echo "Failed to fetch data.\n";
    }
} else {
    echo "Request failed.\n";
}
?>
```

---

## Troubleshooting

### Common Issues

1. **Invalid Serial Number**:
   - Ensure `serialNum` is correct and matches your inverter.

2. **Session Expired**:
   - Re-authenticate using the login process.

3. **Network Issues**:
   - Check your internet connection.

4. **API Errors**:
   - If `success` is `false`, inspect the response payload.

---

## Next Steps

- **Automate**: Use a scheduler to run the script.
- **Visualize**: Import into Grafana or similar.
- **Extend**: Query additional endpoints for more data.

---

## License

_No license specified._
