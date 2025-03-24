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

https://monitor.eg4electronics.com/WManage/web/login

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
