# EG4 Monitoring System: getInverterRuntime Endpoint Documentation

This document explains the fields returned by the `getInverterRuntime` API endpoint and how to access it. The response provides real-time runtime data for the inverter, including voltage, current, power, and status information.

---

## Endpoint Overview

- **URL**: `https://monitor.eg4electronics.com/WManage/api/inverter/getInverterRuntime`
- **Method**: POST
- **Authentication**: Requires a valid session cookie (obtained via the login process).
- **Parameters**: `serialNum` (the serial number of the inverter)

---

## Request

**Headers**:
- `Content-Type: application/x-www-form-urlencoded`
- `X-Requested-With: XMLHttpRequest`
- `Cookie`: Session cookie (obtained after login)

**Body**:
- `serialNum`: The serial number of the inverter

Example request body:
```
serialNum=1234567890
```

---

## Response Fields

The response is a JSON object containing the following fields (selection):

- `success`: `bool` – Indicates if the request was successful
- `serialNum`, `fwCode`, `powerRatingText`
- `lost`, `hasRuntimeData`, `statusText`
- `batteryType`, `batParallelNum`, `batCapacity`
- `model`, `modelText`, `serverTime`, `deviceTime`
- `vpv1`, `vpv2`, `vpv3` – PV input voltages
- `remainTime`, `ppv1`, `ppv2`, `ppv3`
- `vacr`, `vacs`, `vact` – AC output voltages
- `fac` – AC frequency
- `pf` – Power factor
- `vepsr`, `vepss`, `vepst`, `feps`, `seps` – EPS data
- `pToGrid`, `pToUser` – Power exported/imported
- `tinner`, `tradiator1`, `tradiator2`, `tBat`
- `vBus1`, `vBus2`, `status`, `ppv`
- `pCharge`, `pDisCharge`, `batPower`, `batteryColor`, `soc`, `vBat`
- `pinv`, `prec`, `peps`
- `_12KAcCoupleInverterFlow`, `hasEpsOverloadRecoveryTime`
- `maxChgCurr`, `maxDischgCurr`, `maxChgCurrValue`, `maxDischgCurrValue`
- `bmsCharge`, `bmsDischarge`, `bmsForceCharge`
- `_12KUsingGenerator`, `genVolt`, `genFreq`, `genPower`
- `_12KAcCoupleInverterData`, `acCouplePower`, `consumptionPower114`, `pEpsL1N`, `pEpsL2N`, `haspEpsLNValue`
- `genDryContact`, `consumptionPower`, `hasUnclosedQuickChargeTask`

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
  "statusText": "normal",
  "batteryType": "LITHIUM",
  "batParallelNum": "2",
  "batCapacity": "560",
  "model": 623296,
  "modelText": "0x982C0",
  "serverTime": "2025-03-24 01:23:02",
  "deviceTime": "2025-03-23 21:23:02",
  "vpv1": 9,
  "vpv2": 0,
  "vpv3": 42,
  "remainTime": 0,
  "ppv1": 0,
  "ppv2": 0,
  "ppv3": 0,
  "vacr": 2473,
  "vacs": 256,
  "vact": 18,
  "fac": 5998,
  "pf": "1",
  "vepsr": 2473,
  "vepss": 2809,
  "vepst": 15434,
  "feps": 5998,
  "seps": 0,
  "pToGrid": 0,
  "pToUser": 735,
  "tinner": 27,
  "tradiator1": 40,
  "tradiator2": 28,
  "tBat": 1012,
  "vBus1": 3997,
  "vBus2": 3307,
  "status": 0,
  "ppv": 0,
  "pCharge": 0,
  "pDisCharge": 0,
  "batPower": 0,
  "batteryColor": "green",
  "soc": 100,
  "vBat": 550,
  "pinv": 0,
  "prec": 0,
  "peps": 0,
  "_12KAcCoupleInverterFlow": false,
  "hasEpsOverloadRecoveryTime": false,
  "maxChgCurr": 200,
  "maxDischgCurr": 4000,
  "maxChgCurrValue": 20,
  "maxDischgCurrValue": 400,
  "bmsCharge": true,
  "bmsDischarge": true,
  "bmsForceCharge": false,
  "_12KUsingGenerator": false,
  "genVolt": 2472,
  "genFreq": 5998,
  "genPower": 0,
  "_12KAcCoupleInverterData": true,
  "acCouplePower": 0,
  "consumptionPower114": 755,
  "pEpsL1N": 0,
  "pEpsL2N": 0,
  "haspEpsLNValue": true,
  "genDryContact": "OFF",
  "consumptionPower": 755,
  "hasUnclosedQuickChargeTask": false
}
```

---

## Example PHP Code

```php
<?php
$runtimeUrl = 'https://monitor.eg4electronics.com/WManage/api/inverter/getInverterRuntime';
$cookieFile = __DIR__ . '/cookies.txt';

$config = require __DIR__ . '/config.php';
$serialNum = $config['serialNum'];

$ch = curl_init($runtimeUrl);
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
        echo "Battery SOC: " . $data['soc'] . "%\n";
        echo "Battery Voltage: " . ($data['vBat'] / 10) . " V\n";
        echo "PV Voltage: " . ($data['vpv3'] / 10) . " V\n";
        echo "Grid Export Power: " . $data['pToGrid'] . " W\n";
        echo "Grid Import Power: " . $data['pToUser'] . " W\n";
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
   - Ensure the `serialNum` parameter matches your inverter.

2. **Session Expired**:
   - Re-authenticate with the login process.

3. **Network Issues**:
   - Confirm the monitoring server is reachable.

4. **API Errors**:
   - Check if the `success` field is false and review the response.

---

## Next Steps

- **Automate**: Run the script on a schedule to log or monitor real-time data.
- **Visualize**: Feed the data into monitoring dashboards.
- **Extend**: Pair with energy data for full system views.

---

## License

_No license specified._
