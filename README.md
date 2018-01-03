# elasticsearch
PHP7 elasticsearch extension

# Support
php7+

# Install
```
// shell command line
# /usr/local/php/bin/phpize
# ./configure -with-php-config=/usr/local/php/bin/php-config
# make
# make install
```
```
// edit php.ini file
add 'extension=elasticsearch.so'
```

# Documentation
```
function printElasticSearchDocument();
class ElasticSearchClient
    private static $connect_timeout = 1;  // unit:ms  default:1000ms
    private static $request_timeout = 2;  // unit:ms  default:2000ms
    private $host = "";
    private $port = "";
    public $message = "";
    public function __construct(string $host, int $port)
    public function add($params) : mixed
    public function remove($params) : mixed
    public function update($params) : mixed
    public function get($params) : mixed
    public function search($params) : mixed
    public function getMessage() : string
    public static function setConnectTimeout($seconds) : bool
    public static function setRequestTimeout($seconds) : bool
```