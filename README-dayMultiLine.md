# EG4 Monitoring System: dayMultiLine Endpoint Documentation

This document explains the fields returned by the `dayMultiLine` API endpoint. The endpoint provides detailed energy data for a specific day, with measurements taken approximately every 4 minutes (around 360 data points per day).

---

## Endpoint Overview

- **URL**: `https://monitor.eg4electronics.com/WManage/api/analyze/chart/dayMultiLine`
- **Method**: POST
- **Authentication**: Requires valid session cookie
- **Parameters**:
  - `serialNum`: Your inverter's serial number
  - `dateText`: Date in `YYYY-MM-DD` format (e.g., `2025-03-23`)

---

## Response Fields

Each data point includes the following fields:

- `time`: Timestamp of the reading (`YYYY-MM-DD HH:MM:SS`)
- `year`, `month`, `day`, `hour`, `minute`, `second`: Components of the timestamp
- `solarPv`: Solar PV power (watts, positive = generating)
- `gridPower`: Grid power (watts, negative = exporting)
- `batteryDischarging`: Battery power (watts, positive = discharging)
- `consumption`: Total home consumption (watts)
- `soc`: Battery state of charge (percentage)
- `acCouplePower`: AC coupling power (watts)

---

## Example Response Data Point

```json
{
    "time": "2025-01-24 23:58:08",
    "year": 2025,
    "month": 0,
    "day": 24,
    "hour": 23,
    "minute": 58,
    "second": 8,
    "solarPv": 0,
    "gridPower": -755,
    "batteryDischarging": 0,
    "consumption": 755,
    "soc": 100,
    "acCouplePower": 0
}
```

---

## Field Interpretations

- **solarPv**: Power from solar. 0 at night, positive during generation.
- **gridPower**: Negative = exporting to grid, Positive = importing.
- **batteryDischarging**: Positive = battery supplying power, 0 = idle/charging.
- **consumption**: Total home usage in watts.
- **soc**: Battery charge level, range 0–100%.
- **acCouplePower**: Power from AC-coupled sources.

---

## Example PHP Code

```php
<?php
$dayMultiLineUrl = 'https://monitor.eg4electronics.com/WManage/api/analyze/chart/dayMultiLine';
$cookieFile = __DIR__ . '/cookies.txt';

$config = require __DIR__ . '/config.php';
$serialNum = $config['serialNum'];
$dateText = '2025-03-23';

$postData = http_build_query([
    'serialNum' => $serialNum,
    'dateText' => $dateText,
]);

$ch = curl_init($dayMultiLineUrl);
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
    if ($data && isset($data['success']) && $data['success']) {
        foreach ($data['data'] as $point) {
            echo sprintf(
                "%s: Solar=%dW, Grid=%dW, Battery=%dW, Usage=%dW, SOC=%d%%\n",
                $point['time'],
                $point['solarPv'],
                $point['gridPower'],
                $point['batteryDischarging'],
                $point['consumption'],
                $point['soc']
            );
        }
    }
}
?>
```

---

## Data Analysis Tips

1. **Time Intervals**:
   - Data points occur every ~4 minutes
   - Expect ~360 points/day
   - Watch for gaps in the timeline

2. **Power Flow**:
   - Negative `gridPower` = export
   - Net flow = `solarPv + gridPower + batteryDischarging`

3. **Battery Behavior**:
   - Track `soc` and `batteryDischarging` for trends

4. **Daily Trends**:
   - Use `dateText` to compare across days
   - Great for seasonal or performance tracking

---

## Troubleshooting

- **Missing Data**: Inverter may have been offline or communication interrupted
- **Invalid Readings**: Occasionally fields may show outliers; filter if needed
- **Session Issues**: Re-authenticate if session has expired
- **Formatting**: Ensure correct `YYYY-MM-DD` date format

---

## Use Cases

This endpoint is ideal for:

- Home energy audits
- Battery usage tracking
- Export/import monitoring
- Solar generation efficiency checks
- Historical performance comparisons

---

## License

_No license specified._
