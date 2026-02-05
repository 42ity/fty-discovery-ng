# Why DEACT-2.8.5-DEB13 on many tests

Since IPM 2.8.4 on Debian 13, which includes Python 3.13,
the snmpsim package (snmpsimd process) has lost its compatibility with Python 3 due to the removal of deprecated features.

Calls to /usr/bin/snmpsimd consistently fail with an `ImportError` or `ModuleNotFound` exception.

To check, simply try `/usr/bin/snmpsimd --version`...

The use of the snmpsim process by UT has been disabled.
Search for `DEACT-2.8.5-DEB13` tag in code...

## Use snmpsim under python3 virtual environment

See `https://docs.lextudio.com/snmp/#`

new snmpsim python3 package

```bash
$> sudo apt install python3-full python3-pip python3-venv
$> python3 -m venv ./env
$> source ./env/bin/activate
(env)$> pip install snmpsim
(env)$> snmpsim-command-responder --version
(env)$> cd build/
(env)$> cmake ..
(env)$> make && ctest
...
(env)$> deactivate
```

## UT code changes (snmpsim process definition)

snmpsimd process definitions

```cpp
    fty::Process proc("snmpsimd", {
        "--data-dir=root",
        "--agent-udpv4-endpoint=127.0.0.1:1161",
        "--logging-method=file:.snmpsim.txt",
        "--variation-modules-dir=root",
        "--process-user=nobody",
        "--process-group=nogroup",
    });
```

shall be replaced by

```cpp
    fty::Process proc("snmpsim-command-responder", {
        "--data-dir=root",
        "--agent-udpv4-endpoint=127.0.0.1:1161",
        "--logging-method=file:.snmpsim.txt",
        "--variation-modules-dir=root",
    });
```

Notice: `--data-dir` and `--variation-modules-dir` arguments can be `assets`.

