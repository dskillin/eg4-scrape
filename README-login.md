# EG4 Monitoring System: Login Process Documentation

This document explains how to authenticate with the EG4 monitoring system using PHP and cURL. The login process is essential for accessing the system's API endpoints and retrieving data.

---

## Overview

The EG4 monitoring system requires authentication via a web interface. After logging in, a session cookie is generated, which is used for subsequent API requests. This document focuses on the login process.

---

## Prerequisites

- **PHP**: Ensure PHP is installed on your system.
- **cURL**: The PHP cURL extension must be enabled.
- **EG4 Account**: You need a valid EG4 account (username and password).

---

## Login Process

### Step 1: Define Login URL and Credentials

The login URL for the EG4 monitoring system is:  
`https://monitor.eg4electronics.com/WManage/web/login`

You’ll need the following credentials:

- **Account**: Your EG4 account username.  
- **Password**: Your EG4 account password.

Store these credentials securely in a `config.php` file:

```php
<?php
return [
    'account' => 'your-username',
    'password' => 'your-password',
    'serialNum' => 'your-serial-number'
];
?>
```

---

### Step 2: Send Login Request

Use PHP and cURL to send a POST request to the login URL with the account and password.

```php
<?php
$loginUrl = 'https://monitor.eg4electronics.com/WManage/web/login';
$cookieFile = __DIR__ . '/cookies.txt';

$config = require __DIR__ . '/config.php';
$account = $config['account'];
$password = $config['password'];

$loginPostData = http_build_query([
    'account' => $account,
    'password' => $password,
]);

$ch = curl_init($loginUrl);
curl_setopt_array($ch, [
    CURLOPT_RETURNTRANSFER => true,
    CURLOPT_POST => true,
    CURLOPT_POSTFIELDS => $loginPostData,
    CURLOPT_COOKIEJAR => $cookieFile,
    CURLOPT_COOKIEFILE => $cookieFile,
    CURLOPT_FOLLOWLOCATION => true,
]);
$loginResponse = curl_exec($ch);
$loginCode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
curl_close($ch);
?>
```

---

### Step 3: Handle Login Response

Check the HTTP response code to determine if the login was successful:

- **200 or 302**: Login successful. The session cookie is saved in `cookies.txt`.
- **Other codes**: Login failed.

```php
<?php
if ($loginCode === 200 || $loginCode === 302) {
    // Login successful
} else {
    echo "❌ Login failed. HTTP $loginCode\n";
    exit;
}
?>
```

---

### Step 4: Use the Session Cookie

After a successful login, the session cookie is stored in `cookies.txt`. This cookie is used for subsequent API requests to authenticate the session.

Example of using the cookie for an API request:

```php
<?php
$dataUrl = 'https://monitor.eg4electronics.com/WManage/api/inverter/getInverterEnergyInfo';
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
?>
```

---

## Troubleshooting

### Common Issues

1. **Invalid Credentials**:
   - Ensure the account and password are correct.
   - Verify that the account has access to the EG4 monitoring system.

2. **Cookie File Permissions**:
   - Ensure the script has write permissions for the `cookies.txt` file.

3. **Network Issues**:
   - Check your internet connection and ensure the EG4 monitoring system is accessible.

4. **HTTP 302 Redirects**:
   - Enable `CURLOPT_FOLLOWLOCATION` to handle redirects automatically.

---

## Example Code

Here’s the complete PHP code for the login process:

```php
<?php
$loginUrl = 'https://monitor.eg4electronics.com/WManage/web/login';
$cookieFile = __DIR__ . '/cookies.txt';

$config = require __DIR__ . '/config.php';
$account = $config['account'];
$password = $config['password'];

$loginPostData = http_build_query([
    'account' => $account,
    'password' => $password,
]);

$ch = curl_init($loginUrl);
curl_setopt_array($ch, [
    CURLOPT_RETURNTRANSFER => true,
    CURLOPT_POST => true,
    CURLOPT_POSTFIELDS => $loginPostData,
    CURLOPT_COOKIEJAR => $cookieFile,
    CURLOPT_COOKIEFILE => $cookieFile,
    CURLOPT_FOLLOWLOCATION => true,
]);
$loginResponse = curl_exec($ch);
$loginCode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
curl_close($ch);

if ($loginCode === 200 || $loginCode === 302) {
    echo "✅ Login successful.\n";
} else {
    echo "❌ Login failed. HTTP $loginCode\n";
    exit;
}
?>
```

---

## Next Steps

- **Fetch Data**: Use the session cookie to access other API endpoints and retrieve data.
- **Automate**: Set up a cron job or scheduler to run the script periodically.
- **Secure**: Store credentials securely and avoid hardcoding them in the script.

---

## License

_No license specified._
