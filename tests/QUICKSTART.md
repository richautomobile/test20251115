# Quick Start Guide - PSoC4 Integration Testing

Get started with pytest-based integration testing in 5 minutes!

## Prerequisites

- Python 3.8 or higher
- pip (Python package manager)

## 1. Install Dependencies (1 minute)

```bash
cd tests
pip install -r requirements.txt
```

## 2. Run Your First Test (1 minute)

```bash
# Run smoke tests (fastest)
./run_tests.sh smoke
```

Expected output:
```
===============================================
PSoC4 Integration Test Runner
===============================================

Test Category: smoke
-----------------------------------------------

tests/integration/test_system_integration.py ....    [100%]

✓ All tests passed!
===============================================
```

## 3. Explore Different Test Categories (2 minutes)

```bash
# Run all tests
./run_tests.sh all

# Run only unit tests
./run_tests.sh unit

# Run only integration tests
./run_tests.sh integration

# Run peripheral-specific tests
./run_tests.sh i2c
./run_tests.sh adc
./run_tests.sh pwm
```

## 4. Generate Coverage Report (1 minute)

```bash
./run_tests.sh coverage
```

Then open `htmlcov/index.html` in your browser to see detailed coverage.

## 5. Write Your First Test

Create a new file `tests/integration/test_my_feature.py`:

```python
import pytest
from mocks.peripheral_mocks import I2CMock

@pytest.mark.integration
@pytest.mark.i2c
def test_my_i2c_feature():
    """Test my I2C feature."""
    # Setup
    i2c = I2CMock(bus_id=0)
    i2c.init(speed=400000)
    i2c.add_device(0x50)

    # Execute
    result = i2c.write(0x50, 0x00, b"\x01\x02\x03")

    # Verify
    assert result == 0
    data = i2c.read(0x50, 0x00, 3)
    assert data == b"\x01\x02\x03"
```

Run your test:
```bash
pytest tests/integration/test_my_feature.py -v
```

## Common Commands Reference

```bash
# Quick smoke test
make test-smoke

# Run all tests
make test

# Run with coverage
make coverage

# Clean artifacts
make clean

# See all available targets
make help
```

## Next Steps

1. Read the full [README.md](README.md) for detailed documentation
2. Explore existing tests in `tests/integration/` and `tests/unit/`
3. Review available mocks in `tests/mocks/`
4. Check out fixtures in `tests/conftest.py`

## Troubleshooting

**Problem:** "pytest: command not found"
**Solution:** Run `pip install -r requirements.txt`

**Problem:** Import errors
**Solution:** Make sure you're in the tests directory

**Problem:** Permission denied on run_tests.sh
**Solution:** Run `chmod +x run_tests.sh`

---

Need help? Check the [README.md](README.md) or existing test examples!
