# EG4 Monitoring System: Inverter List Endpoint Documentation

This document explains the fields returned by the `inverter/list` API endpoint. This endpoint provides detailed information about the inverters associated with your account, including configuration, status, and specifications.

---

## Endpoint Overview

- **URL**: `https://monitor.eg4electronics.com/WManage/web/config/inverter/list`
- **Method**: POST
- **Authentication**: Requires valid session cookie
- **Parameters**:
  - `page`: Page number for pagination (e.g., `1`)
  - `rows`: Number of entries per page (e.g., `20`)
  - `plantId`: Installation/Plant ID
  - `searchText`: Optional search filter
  - `targetSerialNum`: Target inverter serial number

---

## Response Structure

The response is a JSON object containing:

- `total`: Total number of results
- `currentPageSize`: Number of results in current page
- `offlineNum`: Number of offline inverters
- `onlineNum`: Number of online inverters
- `showAutoParallelButton`: Whether parallel config is shown
- `rows`: Array of inverter objects

### Each Inverter Object Includes:

**Basic Info**:
- `serialNum`: Inverter serial
- `datalogSn`: Logger serial
- `plantId`, `plantName`, `endUser`: Installation details (sanitized)

**Technical Specs**:
- `phase`, `deviceType`, `deviceTypeText`
- `batteryType`, `batteryTypeText`
- `subDeviceType`, `model`
- `powerRating`, `powerRatingText`

**Firmware & Versions**:
- `fwCode`, `standard`
- `slaveVersion`, `fwVersion`, `hardwareVersion`
- `protocolVersion`

**Status**:
- `status`, `statusText`, `lost`
- `allowExport2Grid`, `setG98Already`
- `dtc`, `voltClass`, `machineType`

**Battery Configuration**:
- `batParallelNum`, `batCapacity`, `sohText`

**Integration & Misc**:
- `withbatteryData`
- `bindTigoInfo`, `bindPhnixInfo`, `bindChargePoint`
- `meterType`, `parallelEnabled`

**Location & Timestamps**:
- `country`, `countryText`, `serverId`
- `lastUpdateTime`: `YYYY-MM-DD HH:MM:SS`

---

## Example Response (Sanitized)

```json
{
  "total": 1,
  "rows": [
    {
      "serialNum": "yourserial",
      "datalogSn": "sanitized-logger-id",
      "phase": 1,
      "deviceType": 6,
      "deviceTypeText": "18KPV",
      "batteryType": "LITHIUM",
      "batteryTypeText": "Lithium battery",
      "subDeviceType": 164,
      "allowExport2Grid": 1,
      "dtc": 1,
      "voltClass": 0,
      "machineType": 0,
      "model": 623296,
      "fwCode": "fAAB-1E1E",
      "status": 0,
      "statusText": "Standby",
      "powerRating": 6,
      "powerRatingText": "12kW",
      "standard": "fAAB",
      "slaveVersion": 30,
      "fwVersion": 30,
      "hardwareVersion": -1,
      "withbatteryData": true,
      "protocolVersion": 5,
      "meterType": 0,
      "country": "UNITED_STATES_OF_AMERICA",
      "countryText": "United States of America",
      "parallelEnabled": true,
      "plantId": 9999,
      "plantName": "Sanitized Plant",
      "endUser": "user@example.com",
      "batParallelNum": "2",
      "batCapacity": "560",
      "sohText": "100%",
      "serverId": 1,
      "lastUpdateTime": "2025-03-24 00:26:38"
    }
  ],
  "currentPageSize": 1,
  "offlineNum": 0,
  "onlineNum": 1,
  "showAutoParallelButton": false
}
```

---

## Example PHP Code

```php
<?php
$listUrl = 'https://monitor.eg4electronics.com/WManage/web/config/inverter/list';
$cookieFile = __DIR__ . '/cookies.txt';

$config = require __DIR__ . '/config.php';
$serialNum = $config['serialNum'];

$postData = http_build_query([
    'page' => 1,
    'rows' => 20,
    'plantId' => '9999',
    'searchText' => '',
    'targetSerialNum' => $serialNum
]);

$ch = curl_init($listUrl);
curl_setopt_array($ch, [
    CURLOPT_RETURNTRANSFER => true,
    CURLOPT_POST => true,
    CURLOPT_POSTFIELDS => $postData,
    CURLOPT_HTTPHEADER => [
        'Content-Type: application/x-www-form-urlencoded; charset=UTF-8',
        'X-Requested-With: XMLHttpRequest'
    ],
    CURLOPT_COOKIEFILE => $cookieFile,
]);

$response = curl_exec($ch);
curl_close($ch);

if ($response) {
    $data = json_decode($response, true);
    if ($data && isset($data['rows'][0])) {
        $inverter = $data['rows'][0];
        echo "Inverter Status: " . $inverter['statusText'] . "\n";
        echo "Model: " . $inverter['powerRatingText'] . "\n";
        echo "Firmware: " . $inverter['fwCode'] . "\n";
        echo "Battery: " . $inverter['batteryTypeText'] .
             " (" . $inverter['batParallelNum'] . " parallel, " .
             $inverter['batCapacity'] . "Ah)\n";
        echo "Last Update: " . $inverter['lastUpdateTime'] . "\n";
    }
}
?>
```

---

## Usage Notes

1. **Status Codes**:
   - Use `statusText` for readability
   - `lost` indicates connection issues

2. **Battery Details**:
   - `batParallelNum` and `batCapacity` define the config
   - `sohText` reflects battery health

3. **Grid Interaction**:
   - `allowExport2Grid = 1` means export is enabled

4. **Firmware Tracking**:
   - Monitor `fwCode`, `standard`, and `fwVersion`

5. **System Health**:
   - Check `lastUpdateTime` for freshness
   - Compare `onlineNum` vs. `offlineNum`

---

## Recommended Use Cases

- Checking inverter status and last contact
- Confirming battery specs and health
- Verifying installation metadata
- Auditing firmware and hardware configuration

---

## License

_No license specified._


---

## How to Get Your `plantId`

Currently, the `plantId` is not easily obtained programmatically from the EG4 API. However, you can retrieve it manually by inspecting network traffic in your browser:

### Steps:

1. **Open the EG4 Monitoring Site**  
   Go to: [https://monitor.eg4electronics.com](https://monitor.eg4electronics.com) and log in to your account.

2. **Open Developer Tools**  
   - Right-click anywhere on the page and select **Inspect**, or press `F12` on your keyboard.
   - Navigate to the **Network** tab.

3. **Filter by `Fetch/XHR`**  
   - In the **Network** panel, filter by `Fetch/XHR` to narrow down background requests.

4. **Look for `list` Requests**  
   - Reload the page or click on the "Monitor" section if necessary.
   - Find a request with the name `list` under the `Name` column. The URL will look like:
     ```
     https://monitor.eg4electronics.com/WManage/web/config/inverter/list
     ```

5. **Inspect the Payload or Response**  
   - Click on the `list` request.
   - In the **Response** tab, you’ll see a JSON object containing a field named `plantId` for each inverter.
   - You may also find it in the **Headers** or **Payload** under the `Form Data`.

### Example:

```json
{
    "plantId": 9999,
    "plantName": "MyHome",
    ...
}
```

Copy that `plantId` and use it in your API calls.

---

_Programmatic retrieval of `plantId` is a work in progress. Future updates may include an automated solution._
