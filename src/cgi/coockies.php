<?php
// Set a cookie with name "user" and value "JohnDoe" that expires in 1 hour
$expire_time = time() + 3600; // Current time + 1 hour

// Output a message after setting the cookie
echo setcookie("user", "JohnDoe", $expire_time, "/");
?>