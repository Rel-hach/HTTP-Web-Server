<?php
function generateToken($name, $lastName) {
    // Concatenate the name and last name
    $fullName = $name . $lastName;

    // Hash the concatenated string using SHA-256
    $token = hash('sha256', $fullName);

    return $token;
}

// Example usage:
$name = "John";
$lastName = "Doe";
$token = generateToken($name, $lastName);
echo "Generated Token: " . $token;
?>