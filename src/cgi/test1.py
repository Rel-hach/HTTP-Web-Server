import os

# Accessing an environment variable by name
env_variable_name = "name"
env_variable_value = os.environ.get(env_variable_name)

if env_variable_value is not None:
    print(f"Environment variable {env_variable_name} has value: {env_variable_value}")
else:
    print(f"Environment variable {env_variable_name} not found.")