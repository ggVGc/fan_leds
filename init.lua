clear_display()

set_builtin_led(1)

counter = 0
layer = 0
layer_counter = 0
dir = true

function loop()

clear_display()
  counter = counter + 1
  layer_counter = layer_counter + 1

    set_point(layer, 0, 1)
    set_point(31, 5, 1)

  if counter == 8 then
    counter = 0
    blink_point(0, 5)
  end

  if layer_counter >= 0 then
    layer_counter = 0
    if dir then
      layer = layer + 1

      if layer > 32 then
        layer = 32
        dir = not dir
      end
    else
      layer = layer - 1

      if layer < 0 then
        layer = 0
        dir = not dir
      end

    end
  end

  end_frame()
end
