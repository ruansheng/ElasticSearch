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
    public static $connect_timeout = 1000;  // unit:ms  default:1000ms
    public static $request_timeout = 2000;  // unit:ms  default:2000ms
    private $host = "";
    private $port = "";
    private $message = "";
    public function __construct(string $host, int $port)
    public function add($params) : mixed
    public function remove($params) : mixed
    public function update($params) : mixed
    public function get($params) : mixed
    public function search($params) : mixed
    public function getMessage() : string
    public static function setConnectTimeout($millisecond) : bool
    public static function setRequestTimeout($millisecond) : bool
```

# Example
```
// echo this extension Documentation
php -r 'printElasticSearchDocument();'

$client = new ElasticSearchClient("demo.es.com", 9023);
var_dump($client);

// setConnectTimeout static method example
ElasticSearchClient::setConnectTimeout(1000);

// setRequestTimeout static method example
ElasticSearchClient::setRequestTimeout(2000);

// add method example
$params = [    // es server auto create id
   'index' => 'testindex',
   'type' => 'testtype',
   'body' => ['a' => '1', 'b'=> 2]
];
$params = [    // user defined id
   'index' => 'testindex',
   'type' => 'testtype',
   'id' => 123,  // 'id' => '123',  id type is int or string
   'body' => ['a' => '1', 'b'=> 2]
];
$ret = $client->add($params);
if(!$ret) {
    $message = $client->getMessage();
} else {
    var_dump($ret);
}

// remove method example
$params = [
   'index' => 'testindex',
   'type' => 'testtype',
   'id' => 123,  // 'id' => '123',  id type is int or string
];
$ret = $client->remove($params);
if(!$ret) {
    $message = $client->getMessage();
} else {
    var_dump($ret);
}

// update method example
$params = [
   'index' => 'testindex',
   'type' => 'testtype',
   'id' => 123,  // 'id' => '123',  id type is int or string
   'body' => [   // body is elasticsearch update api identical
       "doc" => [
            "name" : "new_name"
        ],
        "doc_as_upsert" => true
   ]
];

/*
$params['body'] = [
    "script" => [
        "inline" => "ctx._source.counter += count",
        "params" => [
            "count" => 4
        ]
     ]
];
*/

/*
$params['body'] = [
    "script" => [
        "inline" => "ctx._source.counter += count",
        "params" => [
            "count" => 4
        ]
     ]
];
*/

$ret = $client->update($params);
if(!$ret) {
    $message = $client->getMessage();
} else {
    var_dump($ret);
}

// get method example
$params = [
   'index' => 'testindex',
   'type' => 'testtype',
   'id' => 123,  // 'id' => '123',  id type is int or string
];
$ret = $client->get($params);
if(!$ret) {
    $message = $client->getMessage();
} else {
    var_dump($ret);
}

// search method example
$params = [
   'index' => 'testindex',
   'type' => 'testtype',
    'body' => [
        'query' => [
            'match_all' => []
        ],
        'from' => 2,
        'size' => 10,
        'sort' => [
            'price' => [
            'order' => 'asc'
            ]
    	]
    ]
];
$ret = $client->search($params);
if(!$ret) {
    $message = $client->getMessage();
} else {
    var_dump($ret);
}
```