#!/usr/bin/env bash

# taken from https://superuser.com/a/284226

curl -L  -H "Accept: application/vnd.github+json"  -H "Authorization: Bearer ${{ github.TOKEN }}" -H "X-GitHub-Api-Version: 2022-11-28" https://api.github.com/repos/de-xterm/cppli/actions/caches > output.json

jq -r .actions_caches[].key output.json> plain.txt


while IFS= read -r line; do
    curl -L  -X DELETE  -H "Accept: application/vnd.github+json"  -H "Authorization: Bearer ${{ github.TOKEN }}" -H "X-GitHub-Api-Version: 2022-11-28" https://api.github.com/repos/de-xterm/cppli/actions/caches?key=$line
    echo deleted cache \"$line\"
    echo
done < "plain.txt"

