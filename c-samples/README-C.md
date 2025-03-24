# EG4 Monitoring System: C Implementation Documentation

This document explains how to set up, compile, and use the **C-based command-line interface** for the EG4 Monitoring System API.

---

## 📁 Required Files

**Source Files:**
- `eg4_api.h` – Header file with function declarations
- `eg4_api.c` – Main implementation
- `cJSON.h` / `cJSON.c` – JSON parsing library
- `config.txt` – User configuration

**Dependencies:**
- `libcurl`
- `cJSON` (from Dave Gamble)
- `gcc` or `clang`

---

## 💻 Installation on macOS (ARM)

### 1. Install Dependencies
```bash
brew install curl
```

### 2. Get `cJSON`
```bash
curl -O https://raw.githubusercontent.com/DaveGamble/cJSON/master/cJSON.h
curl -O https://raw.githubusercontent.com/DaveGamble/cJSON/master/cJSON.c
```

### 3. Compile the Program
```bash
gcc -o eg4-api eg4_api.c cJSON.c -lcurl -I/opt/homebrew/include -L/opt/homebrew/lib -Wall -Wextra -O2 -arch arm64
```

---

## ⚙️ Configuration

Create a `config.txt` file in the same directory:

```
account=your-username
password=your-password
serialNum=your-serial-number
plantId=your-plant-id
```

---

## 🚀 Usage

### Basic Commands

| Command                | Description                      |
|------------------------|----------------------------------|
| `./eg4-api runtime`    | Get real-time inverter status    |
| `./eg4-api energy`     | Get current energy stats         |
| `./eg4-api month 3 2025` | Get daily data for March 2025 |
| `./eg4-api day 3 3 2025` | Get detailed data for Mar 3     |
| `./eg4-api list`       | Get inverter configuration info  |

---

## 🧾 Command Details

- **runtime**: No arguments. Returns current operational metrics.
- **energy**: No arguments. Returns energy generation and storage data.
- **month `<month>` `<year>`**: Returns per-day values for specified month.
- **day `<day>` `<month>` `<year>`**: Returns ~360 high-res intervals for the day.
- **list**: No arguments. Returns config, firmware, battery status.

---

## 📦 Output Format

All outputs are JSON. Example:

**Runtime:**
```json
{
  "success": true,
  "data": {
    "pvPower": 1234,
    "loadPower": 567,
    "gridPower": -89
  }
}
```

**Monthly:**
```json
{
  "success": true,
  "data": [
    {
      "day": 3,
      "ePv1Day": 12.3,
      "eChgDay": 45.6
    }
  ]
}
```

---

## ❗ Error Handling

- **Login failed**: Check `account/password` in `config.txt`
- **Missing config file**: Ensure `config.txt` exists and is readable
- **cURL errors**: Ensure `libcurl` is installed and network is up
- **Parameter issues**: Verify date formats and argument count

---

## 🐞 Troubleshooting

### Compilation
- Check curl: `brew list curl`
- Confirm headers: `ls /opt/homebrew/include`
- Confirm cJSON files present: `ls cJSON.*`

### Runtime
- Validate `config.txt` format
- Check write access for `cookies.txt`
- Verify API site is reachable

---

## 🧪 Automation Example

**daily-fetch.sh**
```bash
#!/bin/bash
TODAY=$(date +%d\ %m\ %Y)
./eg4-api day $TODAY > "eg4_data_$(date +%Y%m%d).json"
```

---

## 🔧 Notes

- Session cookie stored in `cookies.txt`
- All API calls require login first
- JSON sent to `stdout`, errors to `stderr`
- Exit code: `0` = success, `1` = error

---

## 📈 Best For

- Headless monitoring
- Cron-based automation
- Command-line diagnostics
- Data ingestion into logging tools

---

Happy monitoring! 🛠️
