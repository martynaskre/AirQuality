# AirQuality Web Server

The web server is responsible for storing air quality measurements and is built using **PHP** and the **Laravel framework**.

## Creating a new device

To register a new device and generate a serial number along with a push key, run:

```bash
php artisan device:provision
```

## Setup

- Copy `.env.example` to `.env` and fill with appropriate values.
