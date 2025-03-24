<?php
/**
 * EG4 API Interface
 * 
 * This script provides functions to interact with the EG4 monitoring system API.
 * By default, it only performs authentication. Uncomment function calls as needed.
 */

// Load configuration
$config = require __DIR__ . '/config.php';
$cookieFile = __DIR__ . '/cookies.txt';

// Base URL
$baseUrl = 'https://monitor.eg4electronics.com/WManage';

/**
 * Performs login and creates cookie file
 */
function eg4_login($baseUrl, $config, $cookieFile) {
    $loginUrl = $baseUrl . '/web/login';
    
    $loginPostData = http_build_query([
        'account' => $config['account'],
        'password' => $config['password'],
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
    $response = curl_exec($ch);
    $httpCode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    curl_close($ch);

    return ($httpCode === 200 || $httpCode === 302);
}

/**
 * Gets current inverter energy info
 */
function eg4_get_energy_info($baseUrl, $config, $cookieFile) {
    $url = $baseUrl . '/api/inverter/getInverterEnergyInfo';
    
    $postData = http_build_query(['serialNum' => $config['serialNum']]);

    $ch = curl_init($url);
    curl_setopt_array($ch, [
        CURLOPT_RETURNTRANSFER => true,
        CURLOPT_POST => true,
        CURLOPT_POSTFIELDS => $postData,
        CURLOPT_COOKIEFILE => $cookieFile,
    ]);
    $response = curl_exec($ch);
    curl_close($ch);

    return json_decode($response, true);
}

/**
 * Gets current inverter runtime data
 */
function eg4_get_runtime($baseUrl, $config, $cookieFile) {
    $url = $baseUrl . '/api/inverter/getInverterRuntime';
    
    $postData = http_build_query(['serialNum' => $config['serialNum']]);

    $ch = curl_init($url);
    curl_setopt_array($ch, [
        CURLOPT_RETURNTRANSFER => true,
        CURLOPT_POST => true,
        CURLOPT_POSTFIELDS => $postData,
        CURLOPT_COOKIEFILE => $cookieFile,
    ]);
    $response = curl_exec($ch);
    curl_close($ch);

    return json_decode($response, true);
}

/**
 * Gets monthly data for specified year/month
 */
function eg4_get_month_column($baseUrl, $config, $cookieFile, $year, $month) {
    $url = $baseUrl . '/api/inverterChart/monthColumn';
    
    $postData = http_build_query([
        'serialNum' => $config['serialNum'],
        'year' => $year,
        'month' => $month
    ]);

    $ch = curl_init($url);
    curl_setopt_array($ch, [
        CURLOPT_RETURNTRANSFER => true,
        CURLOPT_POST => true,
        CURLOPT_POSTFIELDS => $postData,
        CURLOPT_COOKIEFILE => $cookieFile,
    ]);
    $response = curl_exec($ch);
    curl_close($ch);

    return json_decode($response, true);
}

/**
 * Gets detailed day data for specified date
 */
function eg4_get_day_multiline($baseUrl, $config, $cookieFile, $date) {
    $url = $baseUrl . '/api/analyze/chart/dayMultiLine';
    
    $postData = http_build_query([
        'serialNum' => $config['serialNum'],
        'dateText' => $date
    ]);

    $ch = curl_init($url);
    curl_setopt_array($ch, [
        CURLOPT_RETURNTRANSFER => true,
        CURLOPT_POST => true,
        CURLOPT_POSTFIELDS => $postData,
        CURLOPT_COOKIEFILE => $cookieFile,
    ]);
    $response = curl_exec($ch);
    curl_close($ch);

    return json_decode($response, true);
}

/**
 * Gets inverter list
 */
function eg4_get_inverter_list($baseUrl, $config, $cookieFile) {
    $url = $baseUrl . '/web/config/inverter/list';
    
    $postData = http_build_query([
        'page' => 1,
        'rows' => 20,
        'plantId' => $config['plantId'],
        'searchText' => '',
        'targetSerialNum' => $config['serialNum']
    ]);

    $ch = curl_init($url);
    curl_setopt_array($ch, [
        CURLOPT_RETURNTRANSFER => true,
        CURLOPT_POST => true,
        CURLOPT_POSTFIELDS => $postData,
        CURLOPT_COOKIEFILE => $cookieFile,
    ]);
    $response = curl_exec($ch);
    curl_close($ch);

    return json_decode($response, true);
}

// Perform login (required)
if (eg4_login($baseUrl, $config, $cookieFile)) {
    echo "✅ Login successful\n";
} else {
    echo "❌ Login failed\n";
    exit;
}

// Uncomment the following lines to use specific endpoints

// Get current energy info
// $energy = eg4_get_energy_info($baseUrl, $config, $cookieFile);
// print_r($energy);

// Get current runtime data
// $runtime = eg4_get_runtime($baseUrl, $config, $cookieFile);
// print_r($runtime);

// Get monthly data (example: March 2025)
// $monthly = eg4_get_month_column($baseUrl, $config, $cookieFile, 2025, 3);
// print_r($monthly);

// Get detailed day data (example: March 23, 2025)
// $daily = eg4_get_day_multiline($baseUrl, $config, $cookieFile, '2025-03-23');
// print_r($daily);

// Get inverter list
// $list = eg4_get_inverter_list($baseUrl, $config, $cookieFile);
// print_r($list);

?>
