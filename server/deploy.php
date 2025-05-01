<?php

namespace Deployer;

require 'recipe/deploy/update_code.php';
require 'recipe/laravel.php';
require 'contrib/yarn.php';

set('bin/php', function () {
    return which('php8.2');
});

set('application', 'AirQuality');
set('repository', 'git@github.com:martynaskre/AirQuality.git');
set('keep_releases', 2);
set('sub_directory', 'server');

set('shared_files', ['.env', 'public/robots.txt']);

host('airquality.skrebe.me')
    ->set('remote_user', 'airquality')
    ->set('deploy_path', '~/web/airquality.skrebe.me/public_html')
    ->set('http_user', 'www-data')
    ->set('branch', 'master');

task('deploy', [
    'deploy:prepare',
    'deploy:vendors',
    'artisan:config:cache',
    'artisan:route:cache',
    'artisan:event:cache',
    'artisan:migrate',
    'deploy:publish',
]);

after('deploy:failed', 'deploy:unlock');
