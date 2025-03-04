<?php

use App\Api\Http\Controllers\DeviceController;
use App\Api\Http\Controllers\MeasurementController;
use Illuminate\Support\Facades\Route;

Route::get('devices', [DeviceController::class, 'show']);

Route::get('measurements', [MeasurementController::class, 'show']);
Route::post('measurements', [MeasurementController::class, 'store']);
Route::delete('measurements', [MeasurementController::class, 'destroy']);
