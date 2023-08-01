
import sys
import os
# Read the entire content from stdin

content_length = int(os.environ.get('CONTENT_LENGTH', 0))
Quiry = os.environ.get('QUERY_STRING', 0)
request_body = sys.stdin.read(content_length)
# Process the content (if needed)
print("Request body:", request_body, flush=True)