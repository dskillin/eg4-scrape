# EG4 Monitoring System API Documentation

This repository documents key API endpoints for the EG4 Monitoring System, providing PHP examples and field-level explanations for each request and response. It is intended to support developers and power users who want to integrate, monitor, or visualize data from their EG4 inverters.

---

## ⚙️ Sample Usage

You can use the provided `eg4_api.php` script as a base for interacting with the EG4 API.
It supports login and provides wrapper functions for all documented endpoints.

**Files:**
- `eg4_api.php`: Main interface with EG4 endpoints
- `config.php`: Configuration with `account`, `password`, `serialNum`, and `plantId`
- `cookies.txt`: Session storage file used automatically

```php
require 'eg4_api.php';
```

Uncomment the function calls in `eg4_api.php` to fetch energy, runtime, monthly, or daily data,
or to list your registered inverters.

---
## 📘 Available Endpoint Docs

- [`README-login.md`](./README-login.md)  
  Learn how to authenticate with the EG4 monitoring portal using PHP and cURL. Session cookie handling is included.

- [`README-getInverterEnergyInfo.md`](./README-getInverterEnergyInfo.md)  
  Covers energy metrics including solar production, battery charging/discharging, and grid interaction.

- [`README-getInverterRuntime.md`](./README-getInverterRuntime.md)  
  Real-time inverter stats including voltage, frequency, power flows, and battery SOC.

- [`README-monthColumn.md`](./README-monthColumn.md)  
  Historical energy data broken down by day for a given month.

- [`README-dayMultiLine.md`](./README-dayMultiLine.md)  
  High-resolution daily time-series data (every ~4 minutes) for detailed power flow analysis.

- [`README-list.md`](./README-list.md)  
  Retrieves configuration and status info for all inverters associated with your account.

---

## 🔐 Authentication

All endpoints require a valid session cookie, which is obtained by logging in through the `login` endpoint. See [`README-login.md`](./README-login.md) for the full flow.

---

## ⚠️ How to Get Your `plantId`

You currently must extract your `plantId` manually using Chrome DevTools:

1. Log into the EG4 monitoring dashboard at https://monitor.eg4electronics.com
2. Open **Developer Tools** → **Network** tab → **Fetch/XHR**
3. Look for the request to `/config/inverter/list`
4. Inspect the **Form Data** or **Response** for your `plantId`

More detail is in the [README-list.md](./README-list.md) file.

---

## 📜 License

All documentation in this repository is released under [CC0 1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/).

> While attribution is not legally required, a brief acknowledgment of the work involved in deciphering and documenting this integration process is appreciated. A simple thanks goes a long way.

---

## 🤝 Contributing

Feel free to fork, adapt, and improve. Pull requests are welcome, especially for:

- Automating `plantId` discovery
- Adding Python or shell examples
- Converting data to Grafana-compatible formats

---

## 🧰 Tools & Tips

- Use [`cookies.txt`](./cookies.txt) to persist your session cookie across requests.
- Use a scheduler (cron, etc.) to fetch data at regular intervals.
- Combine endpoints to build a full dashboard or monitoring solution.

---

Created with love (and packet sniffing). ❤️
