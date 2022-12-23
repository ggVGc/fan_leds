clear_display()

set_builtin_led(1)

counter = 0
layer = 0
layer_counter = 0
dir = true

cc = 0
a = false
b = false

full_lap_time = 196
lap_time = full_lap_time / 4
on_time = 4
is_on = false
on_counter = 0

-- set_cols(31, 2,1)


function loop()
  cc = cc + 1
  cutoff = 100

  if a then
    cutoff = 10
  end

  if cc > cutoff then
    cc = 0
    a = not a

    if a then
      set_builtin_led(0)
    else
      set_builtin_led(1)
    end
  end

  -- clear_display()
  counter = counter + 1
  if counter >= lap_time then
    counter = 0
    is_on = true
    on_counter = 0
    set_point(0, 5, 1)
  end

  if is_on then
    on_counter = on_counter + 1
    if on_counter > on_time then
      set_point(0, 5, 0)
      is_on = false
    end
  end

  -- layer_counter = layer_counter + 1

  -- set_point(layer, 0, 1)
  -- set_point(31, 5, 1)

  -- if counter == 8 then
  --   counter = 0
  --   blink_point(0, 5)
  -- end

  -- if layer_counter >= 0 then
  --   layer_counter = 0
  --   if dir then
  --     layer = layer + 1

  --     if layer > 32 then
  --       layer = 32
  --       dir = not dir
  --     end
  --   else
  --     layer = layer - 1

  --     if layer < 0 then
  --       layer = 0
  --       dir = not dir
  --     end

  --   end
  -- end

  end_frame()
end
