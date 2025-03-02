<?php

use App\Api\Http\Controllers\MeasurementController;
use Illuminate\Support\Facades\Route;

Route::get('measurements', [MeasurementController::class, 'index']);
Route::post('measurements', [MeasurementController::class, 'store']);
Route::delete('measurements', [MeasurementController::class, 'destroy']);
