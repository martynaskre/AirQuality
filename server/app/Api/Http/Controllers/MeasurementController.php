<?php

namespace App\Api\Http\Controllers;

use App\Api\Http\Requests\Measurements\StoreMeasurementRequest;
use App\Api\Http\Resources\MeasurementResource;
use App\Models\Device;
use Exception;
use Illuminate\Http\Request;
use Illuminate\Http\Response;
use Illuminate\Support\Facades\DB;

class MeasurementController extends Controller
{
    public function index(Request $request): MeasurementResource
    {
        /* @var $device Device */
        $device = $request->get('device');

        return new MeasurementResource(
            $device->measurements()
                ->orderByDesc('calibrated')
                ->orderByDesc('created_at')
                ->firstOrFail()
        );
    }

    public function store(StoreMeasurementRequest $request): Response
    {
        defer(function () use ($request) {
            try {
                DB::beginTransaction();

                /* @var $device Device */
                $device = $request->get('device');

                $device->measurements()
                    ->create([
                        'proximity' => $request->input('proximity'),
                        'lux' => $request->input('lux'),
                        'temperature' => $request->input('temperature'),
                        'pressure' => $request->input('pressure'),
                        'humidity' => $request->input('humidity'),
                        'eco2' => $request->input('eco2'),
                        'tvoc' => $request->input('tvoc'),
                        'raw_h2' => $request->input('raw_h2'),
                        'raw_ethanol' => $request->input('raw_ethanol'),
                        'calibrated' => $request->input('calibrated'),
                    ]);

                DB::commit();
            } catch (Exception $e) {
                DB::rollBack();

                report($e);
            }
        });

        return response(null, 201, [
            'Content-Length' => 0,
        ]);
    }

    public function destroy(Request $request): Response
    {
        defer(function () use ($request) {
            try {
                DB::beginTransaction();

                /* @var $device Device */
                $device = $request->get('device');

                $device->measurements()
                    ->delete();

                DB::commit();
            } catch (Exception $e) {
                DB::rollBack();

                report($e);
            }
        });

        return response(null, 200, [
            'Content-Length' => 0,
        ]);
    }
}
