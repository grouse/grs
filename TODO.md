# TODO
- [ ] expose WS_EX_TOPMOST as API and/or window creation flag, with appropriate window control to toggle
    - can I hook into the right-click context menu for the window titlebar and add a command there?
- [ ] query state for AXIS input events instead of relying on awkward, complicated, and above all broken logic
    - particularly for axes bound to digital events, like keyboard keys
- [ ] query state for HOLD input events intead of relying on delta events to update them
