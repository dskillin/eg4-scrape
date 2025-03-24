# EG4 Monitoring System: monthColumn Endpoint Documentation

This document explains the fields returned by the `monthColumn` API endpoint and how to access it. The response provides daily energy data for the selected month, including solar generation, battery charge/discharge, grid import/export, and usage.

---

## Endpoint Overview

- **URL**: `https://monitor.eg4electronics.com/WManage/api/inverterChart/monthColumn`
- **Method**: POST  
- **Authentication**: Requires a valid session cookie (obtained via the login process)  
- **Parameters**: `serialNum`, `year`, `month`

---

## Request

**Headers**:
- `Accept: application/json, text/javascript, */*; q=0.01`
- `Accept-Language: en-US,en;q=0.9`
- `Cache-Control: no-cache`
- `Connection: keep-alive`
- `Content-Type: application/x-www-form-urlencoded; charset=UTF-8`
- `Origin: https://monitor.eg4electronics.com`
- `Pragma: no-cache`
- `Referer: https://monitor.eg4electronics.com/WManage/web/monitor/inverter`
- `Sec-Fetch-*` and `sec-ch-ua-*` headers as seen in browser traffic
- `X-Requested-With: XMLHttpRequest`
- `Cookie`: Session cookie (obtained after login)

**Body**:
- `serialNum`: Serial number of the inverter
- `year`: Year to query (e.g., `2025`)
- `month`: Month to query (e.g., `3` for March)

Example:
```
serialNum=yourserial&year=2025&month=3
```

---

## Response Fields

Each item in the `data` array corresponds to one day of the month and includes:

- `day`: Day of the month (e.g., 29)
- `ePv1Day`: PV1 energy (kWh)
- `ePv2Day`: PV2 energy (kWh)
- `ePv3Day`: PV3 energy (kWh)
- `eInvDay`: Inverter energy (kWh)
- `eRecDay`: Rectified energy (kWh)
- `eChgDay`: Battery charge (kWh)
- `eDisChgDay`: Battery discharge (kWh)
- `eEpsDay`: EPS output (kWh)
- `eToGridDay`: Grid export (kWh)
- `eToUserDay`: User consumption (kWh)
- `eGenDay`: Generator energy (kWh)
- `eConsumptionDay`: Total consumption (kWh)

---

## Example Response

```json
{
  "success": true,
  "data": [
    {
      "day": 29,
      "ePv1Day": 0,
      "ePv2Day": 0,
      "ePv3Day": 18,
      "eInvDay": 23,
      "eRecDay": 22,
      "eChgDay": 25,
      "eDisChgDay": 11,
      "eEpsDay": 0,
      "eToGridDay": 41,
      "eToUserDay": 338,
      "eGenDay": 109,
      "eConsumptionDay": 407
    },
    {
      "day": 30,
      "ePv1Day": 0,
      "ePv2Day": 0,
      "ePv3Day": 15,
      "eInvDay": 20,
      "eRecDay": 18,
      "eChgDay": 22,
      "eDisChgDay": 9,
      "eEpsDay": 0,
      "eToGridDay": 38,
      "eToUserDay": 325,
      "eGenDay": 105,
      "eConsumptionDay": 395
    }
  ]
}
```

---

## Example PHP Code

```php
<?php
$monthColumnUrl = 'https://monitor.eg4electronics.com/WManage/api/inverterChart/monthColumn';
$cookieFile = __DIR__ . '/cookies.txt';

$config = require __DIR__ . '/config.php';
$serialNum = $config['serialNum'];

// Set the year and month
$year = 2025;
$month = 3;

$postData = http_build_query([
    'serialNum' => $serialNum,
    'year' => $year,
    'month' => $month,
]);

$ch = curl_init($monthColumnUrl);
curl_setopt_array($ch, [
    CURLOPT_RETURNTRANSFER => true,
    CURLOPT_POST => true,
    CURLOPT_POSTFIELDS => $postData,
    CURLOPT_HTTPHEADER => [
        'Accept: application/json, text/javascript, */*; q=0.01',
        'Accept-Language: en-US,en;q=0.9',
        'Cache-Control: no-cache',
        'Connection: keep-alive',
        'Content-Type: application/x-www-form-urlencoded; charset=UTF-8',
        'Origin: https://monitor.eg4electronics.com',
        'Pragma: no-cache',
        'Referer: https://monitor.eg4electronics.com/WManage/web/monitor/inverter',
        'Sec-Fetch-Dest: empty',
        'Sec-Fetch-Mode: cors',
        'Sec-Fetch-Site: same-origin',
        'User-Agent: Mozilla/5.0 (compatible)',
        'X-Requested-With: XMLHttpRequest',
        'sec-ch-ua: \"Chromium\";v=\"134\", \"Not:A-Brand\";v=\"24\", \"Google Chrome\";v=\"134\"',
        'sec-ch-ua-mobile: ?0',
        'sec-ch-ua-platform: \"macOS\"',
    ],
    CURLOPT_COOKIEFILE => $cookieFile,
]);
$response = curl_exec($ch);
curl_close($ch);

if ($response) {
    $data = json_decode($response, true);
    if ($data && isset($data['success']) && $data['success']) {
        echo "Monthly data retrieved successfully:\n";
        print_r($data);
    } else {
        echo "Failed to fetch monthly data.\n";
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
   - Check `serialNum` is valid and corresponds to your inverter.

2. **Session Expired**:
   - Re-login and obtain a fresh session cookie.

3. **Network Problems**:
   - Ensure the EG4 system is reachable.

4. **API Errors**:
   - If `success` is false, inspect the full response body.

---

## Next Steps

- **Automate**: Use a scheduler to poll monthly data.
- **Visualize**: Build dashboards with tools like Grafana.
- **Explore**: Pair with runtime or energy endpoints for deeper insight.

---

## License

_No license specified._
