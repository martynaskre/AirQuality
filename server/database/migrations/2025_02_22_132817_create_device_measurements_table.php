<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

return new class extends Migration
{
    /**
     * Run the migrations.
     */
    public function up(): void
    {
        Schema::create('device_measurements', function (Blueprint $table) {
            $table->uuid('id')->primary();
            $table->foreignUuid('device_id')
                ->constrained('devices')
                ->cascadeOnDelete();
            $table->smallInteger('proximity')
                ->unsigned();
            $table->double('lux');
            $table->double('temperature');
            $table->double('pressure');
            $table->double('humidity');
            $table->smallInteger('eco2')
                ->unsigned();
            $table->smallInteger('tvoc')
                ->unsigned();
            $table->smallInteger('raw_h2')
                ->unsigned();
            $table->smallInteger('raw_ethanol')
                ->unsigned();
            $table->boolean('calibrated');
            $table->timestamps();
        });
    }

    /**
     * Reverse the migrations.
     */
    public function down(): void
    {
        Schema::dropIfExists('devices_measurements');
    }
};
