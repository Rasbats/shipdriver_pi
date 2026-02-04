TOOLS README
============

This file collects some notes on available tool integrations

pre-commit
----------

The pre-commit project at  https://pre-commit.com/ offers a flexible
configuration of git pre-commit hooks. The template contains the file
file *.pre-commit.yaml* which is a canned configuration handling white
space fixes and, for source files, cmake-format.

The basic way to use this is to make sure the python, including pip, is
available. On Linux this is normally out of the box, other OS:s needs
separate steps outside the scope of this document.

With python/pip in place do:

  - pip install --user pre-commit
  - pre-commit install

This makes checks run when doing a `git commit`. Occasionally they cause
trouble; then use `git commit --no-check` instead.
