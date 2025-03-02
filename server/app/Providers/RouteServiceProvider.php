<?php

namespace App\Providers;

use App\Api\Http\Middleware\AuthorizeDevice;
use Illuminate\Foundation\Support\Providers\RouteServiceProvider as ServiceProvider;
use Illuminate\Support\Facades\Route;

class RouteServiceProvider extends ServiceProvider
{
    /**
     * Define your route model bindings, pattern filters, and other route configuration.
     */
    public function boot(): void
    {
        $this->routes(function (): void {
            Route::prefix('api')
                ->middleware(['api', 'auth.device'])
                ->group(base_path('routes/api.php'));
        });
    }
}
