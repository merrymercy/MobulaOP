from .register import register
import mobula_op

@register
class ROIAlign:
    def __init__(self, pooled_size, spatial_scale, sampling_ratio):
        self.pooled_size = pooled_size
        self.spatial_scale = spatial_scale
        self.sampling_ratio = sampling_ratio
    def forward(self, data, rois):
        if self.req[0] == 'null':
            return
        out = self.y
        if self.req[0] == 'add':
            out_temp = self.F.empty_like(out)
            mobula_op.func.roi_align_forward(out.size, data, self.spatial_scale, data.shape[1], data.shape[2], data.shape[3], self.pooled_size[0], self.pooled_size[1], self.sampling_ratio, rois, out_temp)
            self.y[:] += out_temp
        else:
            mobula_op.func.roi_align_forward(out.size, data, self.spatial_scale, data.shape[1], data.shape[2], data.shape[3], self.pooled_size[0], self.pooled_size[1], self.sampling_ratio, rois, self.y)
    def backward(self, dy):
        if self.req[0] == 'null':
            return
        if self.req[0] != 'add':
            self.dX[0][:] = 0
        data, rois = self.X
        mobula_op.func.roi_align_backward(dy.size, dy, rois.shape[0], self.spatial_scale, data.shape[1], data.shape[2], data.shape[3], self.pooled_size[0], self.pooled_size[1], self.sampling_ratio, self.dX[0], rois)
        if self.req[1] != 'null' and self.req[1] != 'add':
            self.dX[1][:] = 0
    def infer_shape(self, in_shape):
        dshape, rshape = in_shape
        assert len(dshape) == 4
        assert len(rshape) == 2
        assert rshape[1] == 5
        oshape = [rshape[0], dshape[1], self.pooled_size[0], self.pooled_size[1]]
        return [dshape, rshape], [oshape]
