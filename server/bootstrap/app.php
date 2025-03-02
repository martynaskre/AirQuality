<?php

use App\Api\Http\Middleware\AuthorizeDevice;
use App\Api\Http\Middleware\ForceJsonResponse;
use Illuminate\Foundation\Application;
use Illuminate\Foundation\Configuration\Exceptions;
use Illuminate\Foundation\Configuration\Middleware;

return Application::configure(basePath: dirname(__DIR__))
    ->withRouting(
        health: '/up',
    )
    ->withMiddleware(function (Middleware $middleware) {
        $middleware->api(
            prepend: [ForceJsonResponse::class]
        );
        $middleware->alias([
            'auth.device' => AuthorizeDevice::class,
        ]);
    })
    ->withExceptions(function (Exceptions $exceptions) {
        //
    })->create();
