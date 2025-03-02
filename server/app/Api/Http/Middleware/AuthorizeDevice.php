<?php

namespace App\Api\Http\Middleware;

use App\Models\Device;
use Closure;
use Illuminate\Http\Request;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\HttpKernel\Exception\UnauthorizedHttpException;

class AuthorizeDevice
{
    /**
     * Handle an incoming request.
     *
     * @param Closure(Request): (Response) $next
     */
    public function handle(Request $request, Closure $next): Response
    {
        $device = Device::query()
            ->where('identifier', $request->header('Station-Number'))
            ->first();

        abort_if($device === null, 401);

        if (in_array($request->method(), ['POST', 'PUT', 'PATCH', 'DELETE'])) {
            abort_if($device->push_key !== $request->header('Push-Key'), 401);
        }

        $request->request->set('device', $device);

        return $next($request);
    }
}
